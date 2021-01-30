#pragma once

#include "AStarNode.h"

namespace Algs{
	enum struct AStarNodeType: int{
		Accessible,
		Inaccessible,
		Amt
	};

	template <class T, typename Type>
	struct CreateAStarNodeParams;

	template <class T = Vec<float, 3>, typename Type = float>
	class AStar final{
	public:
		AStar<T, Type>();
		~AStar<T, Type>();

		AStarNode<T, Type>* CreateNode(const CreateAStarNodeParams<T, Type>& params);
		bool CalcShortestPath();
		void PrintPath();
		void Reset();

		AStarNode<T, Type>* FindNode(const T& pos){
			for(AStarNode<T, Type>* const element: toVisit){
				if(element->pos == pos){
					return element;
				}
			}

			assert(false && "Node with 'pos' canot be found");
			return nullptr;
		}

		void SetStart(const T& pos){
			const size_t mySize = toVisit.size();

			for(size_t i = 0; i < mySize; ++i){
				AStarNode<T, Type>* const element = toVisit[i];

				if(element->pos == pos){
					if(start){
						assert(false);
					}

					start = element;
					toVisit.erase(toVisit.begin() + i);
					toVisit.insert(toVisit.begin(), start);

					return;
				}
			}
			assert(false);
		}

		void SetEnd(const T& pos){
			const size_t mySize = toVisit.size();

			for(size_t i = 0; i < mySize; ++i){
				AStarNode<T, Type>* const element = toVisit[i];

				if(element->pos == pos){
					if(end){
						assert(false);
					}

					end = element;
					toVisit.erase(toVisit.begin() + i);
					toVisit.emplace_back(end);

					return;
				}
			}
			assert(false);
		}

		void SetNeighboursForSquareGrid(const T& bottomLeft, const T& bottomRight, const T& topLeft, const T& topRight, const Type& gridCellWidth, const Type& gridCellHeight){
			for(AStarNode<T, Type>* const element: toVisit){
				element->ReserveNeighbours(4);

				if(element->pos.x > bottomLeft.x){
					element->AddNeighbour(FindNode(T(element->pos.x - gridCellWidth, element->pos.y, element->pos.z)));
				}
				if(element->pos.x < topRight.x){
					element->AddNeighbour(FindNode(T(element->pos.x + gridCellWidth, element->pos.y, element->pos.z)));
				}
				if(element->pos.y > bottomRight.y){
					element->AddNeighbour(FindNode(T(element->pos.x, element->pos.y - gridCellHeight, element->pos.z)));
				}
				if(element->pos.y < topLeft.y){
					element->AddNeighbour(FindNode(T(element->pos.x, element->pos.y + gridCellHeight, element->pos.z)));
				}
			}
		}

		void SetNeighboursForHexGridFlatTop(const T& bottomLeft, const T& bottomRight, const T& topLeft, const T& topRight, const Type& gridCellWidth, const Type& gridCellHeight){
			for(AStarNode<T, Type>* const element: toVisit){
				element->ReserveNeighbours(6);

				if(element->pos.x > bottomLeft.x){
					element->AddNeighbour(FindNode(T(element->pos.x - gridCellWidth, element->pos.y, element->pos.z)));
				}
				if(element->pos.x < topRight.x){
					element->AddNeighbour(FindNode(T(element->pos.x + gridCellWidth, element->pos.y, element->pos.z)));
				}
				if(element->pos.y > bottomRight.y){
					element->AddNeighbour(FindNode(T(element->pos.x, element->pos.y - gridCellHeight, element->pos.z)));
				}
				if(element->pos.y < topLeft.y){
					element->AddNeighbour(FindNode(T(element->pos.x, element->pos.y + gridCellHeight, element->pos.z)));
				}

				if((int)element->pos.x & 1){
					if(element->pos.x > bottomLeft.x && element->pos.y < topLeft.y){
						element->AddNeighbour(FindNode(T(element->pos.x - gridCellWidth, element->pos.y + gridCellHeight, element->pos.z)));
					}
					if(element->pos.x < topRight.x && element->pos.y < topLeft.y){
						element->AddNeighbour(FindNode(T(element->pos.x + gridCellWidth, element->pos.y + gridCellHeight, element->pos.z)));
					}
				} else{
					if(element->pos.x > bottomLeft.x && element->pos.y > bottomRight.y){
						element->AddNeighbour(FindNode(T(element->pos.x - gridCellWidth, element->pos.y - gridCellHeight, element->pos.z)));
					}
					if(element->pos.x < topRight.x && element->pos.y > bottomRight.y){
						element->AddNeighbour(FindNode(T(element->pos.x + gridCellWidth, element->pos.y - gridCellHeight, element->pos.z)));
					}
				}
			}
		}

		void SetNeighboursForHexGridSharpTop(const T& bottomLeft, const T& bottomRight, const T& topLeft, const T& topRight, const Type& gridCellWidth, const Type& gridCellHeight){
			for(AStarNode<T, Type>* const element: toVisit){
				element->ReserveNeighbours(6);

				if(element->pos.x > bottomLeft.x){
					element->AddNeighbour(FindNode(T(element->pos.x - gridCellWidth, element->pos.y, element->pos.z)));
				}
				if(element->pos.x < topRight.x){
					element->AddNeighbour(FindNode(T(element->pos.x + gridCellWidth, element->pos.y, element->pos.z)));
				}
				if(element->pos.y > bottomRight.y){
					element->AddNeighbour(FindNode(T(element->pos.x, element->pos.y - gridCellHeight, element->pos.z)));
				}
				if(element->pos.y < topLeft.y){
					element->AddNeighbour(FindNode(T(element->pos.x, element->pos.y + gridCellHeight, element->pos.z)));
				}

				if((int)element->pos.y & 1){
					if(element->pos.x < topRight.x && element->pos.y > bottomRight.y){
						element->AddNeighbour(FindNode(T(element->pos.x + gridCellWidth, element->pos.y - gridCellHeight, element->pos.z)));
					}
					if(element->pos.x < topRight.x && element->pos.y < topLeft.y){
						element->AddNeighbour(FindNode(T(element->pos.x + gridCellWidth, element->pos.y + gridCellHeight, element->pos.z)));
					}
				} else{
					if(element->pos.x > bottomLeft.x && element->pos.y > bottomRight.y){
						element->AddNeighbour(FindNode(T(element->pos.x - gridCellWidth, element->pos.y - gridCellHeight, element->pos.z)));
					}
					if(element->pos.x > bottomLeft.x && element->pos.y < topLeft.y){
						element->AddNeighbour(FindNode(T(element->pos.x - gridCellWidth, element->pos.y + gridCellHeight, element->pos.z)));
					}
				}
			}
		}



		///Getter
		const std::vector<AStarNode<T, Type>*>& GetShortestPath() const;
	private:
		AStarNode<T, Type>* start;
		AStarNode<T, Type>* end;

		std::vector<AStarNode<T, Type>*> toVisit;
		std::vector<AStarNode<T, Type>*> visited;
		std::vector<AStarNode<T, Type>*> shortestPath;

		bool ICalcShortestPath();
		bool FormShortestPath();
	};
};

#include "AStar.inl"