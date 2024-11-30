# Random Weighted Object Generator (RWOG)
The `dzunni::RandomWeightedObjectGenerator` class is a lightweight container and a random object generator class that contains unique elements of type `E` and each element has a weight. `E` must have the requirements for `std::set`. The weight is an unsigned integer that determines each element's probability which equals to _the element's weight divided by the total weight of all elements_. The class uses `mt19937` engine from C++ standard library for randomness. The class needs a seed for the randomizer through its constructor. You can set a new seed by calling `seed()`.

## Constructors
1. `RandomWeightedObjectGenerator(uint seed)` - initializes with a seed for the RNG.

## Methods
### Modifiers
1. `bool insert(const E&, unsigned int)` - inserts an element with their weight and returns `true` if successful, `false` otherwise.
2. `optional<unsigned int> erase(const E&)` - erases an element and returns its weight.
3. `void clear()` - clears the set and the total weight.
4. `optional<unsigne int> modify(const E&, unsigned int)` - modifies the weight of an existing element and returns its last weight.
5. `void update()` - updates the RNG. Call this after using any modifiers.
### Operators
1. `optional<E> operator()` - returns a random element.
### Inquiries
1. `bool empty()` - determines if it is empty. 
2. `size_t size()` - returns the number of elements.
3. `unsigned int totalWeight()` - returns the total weight of all elements.
4. `bool contains()` - determines if the element exists.
5. `unsigned int weight(const E&)` - returns the weight of the element.
6. `probability()` - returns the probability of the element.
### Others
1. `vector<E> sample(size_t amount)` - returns `std::vector` of elements as a sample.

## Aliases
1. `Rwog_i` (int)
2. `Rwog_f` (float)
3. `Rwog_c` (char)
4. `Rwog_d` (double)
5. `Rwog_u` (unsigned int)
6. `Rwog_l` (long)
7. `RwogString` (string)
