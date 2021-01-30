namespace Algs{
	template <class T, typename Type>
	void AStarNode<T, Type>::AddNeighbour(AStarNode<T, Type>* const neighbour){
		neighbours.emplace_back(neighbour);
	}

	template <class T, typename Type>
	void AStarNode<T, Type>::ReserveNeighbours(const size_t size){
		neighbours.reserve(size);
	}

	template <class T, typename Type>
	const T& AStarNode<T, Type>::GetPos() const{
		return pos;
	}

	template <class T, typename Type>
	void AStarNode<T, Type>::SetNeighbours(const std::vector<AStarNode<T, Type>*>& neighbours){
		this->neighbours = neighbours;
	}

	template <class T, typename Type>
	AStarNode<T, Type>::AStarNode():
		accessible(true),
		tag(""),
		pos(T()),
		cost(std::numeric_limits<Type>::max()),
		fCost(std::numeric_limits<Type>::max()),
		gCost(std::numeric_limits<Type>::max()),
		neighbours(),
		parent(nullptr)
	{
	}
}