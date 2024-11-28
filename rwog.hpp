#include <set>
#include <random>
#include <optional>
#include <vector>
using namespace std;
using uint = unsigned int;

namespace dzunni
{
    /**
     * @author dzunni
     * @version 1.0
     * @brief
     * The `dzunni::RandomWeightedObjectGenerator` class is a container and a random object generator class that contains
     * unique elements of type `E` and each element has a weight. `E` must have the requirements for `std::set`. The weight
     * is an unsigned integer that determines each element's probability which equals to the element's weight divided by the
     * total weight of all elements.
     * 
     * The class uses `mt19937` engine from C++ standard library for randomness.
     * 
     * The class needs a seed for the randomizer through its constructor. You can set a new seed by calling `seed()`.
     * 
     * The class offers modifiers: `insert()`, `erase()`, `modify()`, and `clear()`, which modifies the elements and
     * their weight. Make sure that after using any of them, call `update()` to update the randomizer.
     * 
     * You can determine the container's information about itself:
     * 
     * `empty()` - if it is empty;
     * 
     * `size()` - the number of elements contained;
     * 
     * `totalWeight()` - the total weight of all elements;
     * 
     * `contains()` - if it contains a specified element;
     * 
     * Or about its elements:
     * 
     * `weight()` - the weight of the specified element;
     * 
     * `probability()` - the probability of the specified element.
     * 
     * Finally, call its operator `operator()` to generate a random element. Additionally, you can call `sample()`
     * to get a set amount of random elements.
     * 
     * Note: Elements whose weight is zero can be contained but will never be picked by the randomizer.
     */
    template<typename E>
    class RandomWeightedObjectGenerator{
    private:
        using uint = unsigned int;

        mt19937 _rng;
        uniform_int_distribution<uint> _dis;
        uint _total_weight = 0;

        uint getTotalWeight(){
            return _total_weight;
        }

        struct Data {
            uint lower_boundary;
            uint upper_boundary;
            E element;

            Data(const E& e, uint lower_boundary = 0, uint upper_boundary = 0)
            : element(e), lower_boundary(lower_boundary), upper_boundary(upper_boundary)
            {}

            bool operator<(const Data &other_data) const {
                return element < other_data.element;
            }

            uint weight() const {
                return upper_boundary - lower_boundary + 1;
            }
        };

        set<Data> _data_set;

    public:
        RandomWeightedObjectGenerator(uint seed){
            _rng.seed(seed);
        }

        /**
         * @brief Only copies its elements and their weights, and total weight.
         * Call `seed()` and `update()` it after copying.
         */
        RandomWeightedObjectGenerator(const RandomWeightedObjectGenerator &other){
            _data_set = other._data_set;
            _total_weight = other._total_weight;
        }

        RandomWeightedObjectGenerator(RandomWeightedObjectGenerator &&other){
            _data_set = move(other._data_set);
            _total_weight = other._total_weight;
            _rng = move(other._rng);
            _dis = move(other._dis);
        }

        /**
         * @brief The seed for the randomizer.
         */
        void seed(uint seed){
            _rng.seed(seed);
        }

        /**
         * @brief Updates the randomizer. Call this after using `insert()`, `erase()`, `clear()`, and `modify()`.
         */
        void update(){
            if(_total_weight != 0)
                _dis = uniform_int_distribution<uint>(1, _total_weight);
        }

        /**
         * Returns the number of elements.
         */
        size_t size(){
            return _data_set.size();
        }

        /**
         * @brief Determines if it is empty.
         */
        bool empty(){
            return _data_set.empty();
        }

        /**
         * @brief Returns the total weight of all elements.
         */
        uint totalWeight(){
            return _total_weight;
        }

        /**
         * @brief Determines if the element exists.
         */
        bool contains(const E& element){
            return _data_set.find(element) != _data_set.end();
        }

        /**
         * @brief Returns the weight of the element or `nullopt` if the element is not found.
         */
        optional<uint> weight(const E& element){
            auto it = _data_set.find(element);
            if(it == _data_set.end())
                return nullopt;
            return it->weight();
        }

        /**
         * @brief Returns the probability of the element or `nullopt` if the element is not found.
         */
        optional<double> probability(const E& element){
            auto it = _data_set.find(element);
            if(it == _data_set.end())
                return nullopt;
            return (double) it->weight() / _total_weight;
        }

        /**
         * @brief Inserts an element with its weight.
         * @return `true` - successfully added, `false` - there is already an identical element.
         */
        bool insert(const E &element, uint weight){
            if(contains(element))
                return false;
            
            uint lower_boundary = _total_weight + 1;
            uint upper_boundary = _total_weight + weight;
            _total_weight = upper_boundary;

            // cout << "L=" << lower_boundary << ", U=" << upper_boundary << endl;
            _data_set.insert(Data(element, lower_boundary, upper_boundary));
            return true;
        }

        /**
         * @brief Erases an element along with their weight.
         * @return Returns the weight of the element or `nullopt` if the element is not found.
         */
        optional<uint> erase(const E &element){
            auto it = _data_set.find(element);
            if(it != _data_set.end()){
                uint weight = it->weight();
                _total_weight -= weight;
                _data_set.erase(it);
                return weight;
            }
            return nullopt;
        }

        /**
         * @brief Clears the set and the total weight.
         */
        void clear(){
            _data_set.clear();
            _total_weight = 0;
        }

        /**
         * @brief Modifies the weight of an existing element.
         * @return Returns the last weight of the element before modification or `nullopt` if the element is not found.
         */
        optional<uint> modify(const E& element, uint weight){
            auto it = _data_set.find(element);
            if(it != _data_set.end()){
                uint prev_weight = it->weight();
                _total_weight = _total_weight - prev_weight + weight;
                return prev_weight;
            }
            return nullopt;
        }

        /**
         * @brief Returns `std::vector` of elements as a sample.
         */
        vector<E> sample(size_t amount){
            vector<E> ret;
            if(_total_weight != 0){
                E e = *(this->operator()());
                while(amount > 0){
                    ret.push_back(e);
                    --amount;
                }
            }
            return ret;
        }

        /**
         * @brief Returns a random element or `nullopt` if it is empty.
         * Make sure you have called `update()` after modification of the elements before using this operator.
         */
        optional<E> operator()(){
            uint random = _dis(_rng);
            cout << random << endl;
            for(const Data &data : _data_set){
                if(random >= data.lower_boundary && random <= data.upper_boundary)
                    return data.element;
                else
                    cout << "not " << data.element << endl;
            }
            return nullopt;
        }
    };

    using Rwog_c = RandomWeightedObjectGenerator<char>;
    using Rwog_i = RandomWeightedObjectGenerator<int>;
    using Rwog_f = RandomWeightedObjectGenerator<float>;
    using Rwog_d = RandomWeightedObjectGenerator<double>;
    using Rwog_u = RandomWeightedObjectGenerator<unsigned int>;
    using Rwog_l = RandomWeightedObjectGenerator<long>;
    using RwogString = RandomWeightedObjectGenerator<string>;
} // namespace dzunni 
