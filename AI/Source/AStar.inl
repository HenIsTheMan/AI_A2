namespace Algs{
	template <class T, typename Type>
	struct CreateAStarNodeParams final{
		AStarNodeType type = AStarNodeType::Accessible;
		std::string tag = std::string();
		Type cost = Type();
		T pos = T();
	};

	template <class T, typename Type>
	AStar<T, Type>::AStar():
		start(nullptr),
		end(nullptr),
		toVisit(),
		visited(),
		shortestPath()
	{
	}

	template <class T, typename Type>
	AStar<T, Type>::~AStar(){
		Reset();
	}

	template <class T, typename Type>
	AStarNode<T, Type>* AStar<T, Type>::CreateNode(const CreateAStarNodeParams<T, Type>& params){
		AStarNode<T, Type>* const node = new AStarNode<T, Type>();

		node->accessible = !(params.type == AStarNodeType::Inaccessible);
		node->tag = params.tag;
		node->cost = params.cost;
		node->pos = params.pos;

		toVisit.emplace_back(node);
		return node;
	}

	template <class T, typename Type>
	bool AStar<T, Type>::CalcShortestPath(){
		if(!start){
			//puts("No start node in path!");
			return false;
		}
		if(!end){
			//puts("No end node in path!");
			return false;
		}
		if(toVisit.empty()){
			//puts("No nodes in path!");
			return false;
		}
		if(!start->accessible){
			return false;
		}
		if(!end->accessible){
			return false;
		}

		start->gCost = (Type)0;

		return ICalcShortestPath();
	}

	template <class T, typename Type>
	void AStar<T, Type>::PrintPath(){
		std::string pathText;
		const size_t mySize = shortestPath.size();

		for(size_t i = 0; i < mySize; ++i){
			pathText += shortestPath[i]->tag + '\n';
		}
		std::cout << pathText << '\n';
	}

	template <class T, typename Type>
	void AStar<T, Type>::Reset(){
		size_t i;
		const size_t size0 = toVisit.size();
		for(i = 0; i < size0; ++i){
			AStarNode<T, Type>*& node = toVisit[i];
			if(node){
				delete node;
				node = nullptr;
			}
		}

		start = nullptr;
		end = nullptr;

		toVisit.clear();
		visited.clear();
		shortestPath.clear();
	}

	template <class T, typename Type>
	const std::vector<AStarNode<T, Type>*>& AStar<T, Type>::GetShortestPath() const{
		return shortestPath;
	}

	template <class T, typename Type>
	bool AStar<T, Type>::ICalcShortestPath(){
		std::vector<AStarNode<T, Type>*> open;

		open.reserve(toVisit.size());
		visited.reserve(toVisit.size());

		open.emplace_back(start);

		while(true){
			AStarNode<T, Type>* curr = nullptr;
			for(AStarNode<T, Type>* const element: open){
				if(!curr || (curr && element->fCost < curr->fCost)){
					curr = element;
				}
			}

			open.erase(std::find(open.begin(), open.end(), curr));
			visited.emplace_back(curr);

			if(curr == end){
				return FormShortestPath();
			}

			const size_t amtOfNeighbours = curr->neighbours.size();
			for(size_t i = 0; i < amtOfNeighbours; ++i){
				AStarNode<T, Type>* const neighbour = curr->neighbours[i];
				if(!neighbour->accessible || std::find(visited.begin(), visited.end(), neighbour) != visited.end()){
					continue;
				}

				const Type gCostNew = curr->gCost + (curr->pos - neighbour->pos).Length() + curr->cost + neighbour->cost;
				if(gCostNew < neighbour->gCost || std::find(open.begin(), open.end(), neighbour) == open.end()){
					neighbour->gCost = gCostNew;
					neighbour->fCost = neighbour->gCost + (end->pos - neighbour->pos).Length() + end->cost + neighbour->cost;
					neighbour->parent = curr;

					if(std::find(open.begin(), open.end(), neighbour) == open.end()){
						open.emplace_back(neighbour);
					}
				}
			}
		}

		//while(!toVisit.empty()){
		//	AStarNode<T, Type>* node = toVisit.front();
		//	if(node == end){
		//		return FormShortestPath();
		//	}
		//	toVisit.erase(toVisit.begin());
		//	visited.emplace_back(node);

		//	const size_t amtOfNeighbours = node->neighbours.size();
		//	for(size_t i = 0; i < amtOfNeighbours; ++i){
		//		AStarNode<T, Type>* const neighbour = node->neighbours[i];
		//		const Type gCostNew = node->gCost + (node->pos - neighbour->pos).Length();
		//		const auto notInToVisit = std::find(toVisit.begin(), toVisit.end(), neighbour) == toVisit.end();

		//		if((neighbour->accessible && std::find(visited.begin(), visited.end(), neighbour) == visited.end()) && (gCostNew <= neighbour->gCost || notInToVisit)){
		//			neighbour->gCost = gCostNew;
		//			neighbour->fCost = neighbour->gCost + (end->pos - neighbour->pos).Length();
		//			neighbour->parent = node;
		//			if(notInToVisit){
		//				toVisit.emplace_back(neighbour);
		//			}
		//		}
		//	}
		//}
		//return false;
	}

	template <class T, typename Type>
	bool AStar<T, Type>::FormShortestPath(){
		if(!end->parent){
			return false;
		}

		AStarNode<T, Type>* curr = end;
		while(curr){
			shortestPath.emplace_back(curr);
			curr = curr->parent;
		}
		std::reverse(shortestPath.begin(), shortestPath.end());
		return true;
	}
}