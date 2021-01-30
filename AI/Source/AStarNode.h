#pragma once

#include <cassert>
#include <vector>

#include <string>

#include <limits> /////////////////////

template <class Type, int size>
struct Vec;

template <class Type>
struct Vec<Type, 3>;

namespace Algs{
	template <class T, typename Type>
	class AStar;

	template <class T = Vec<float, 3>, typename Type = float>
	class AStarNode final{
		friend AStar<T, Type>;
	public:
		void AddNeighbour(AStarNode<T, Type>* const neighbour);
		void ReserveNeighbours(const size_t size);

		//* Getter
		const T& GetPos() const;
		//*/

		//* Setter
		void SetNeighbours(const std::vector<AStarNode<T, Type>*>& neighbours);
		//*/
	private:
		bool accessible;

		std::string tag;
		T pos;

		Type cost;
		Type fCost;
		Type gCost;

		std::vector<AStarNode<T, Type>*> neighbours;
		AStarNode<T, Type>* parent;

		AStarNode<T, Type>();
	};
}

#include "AStarNode.inl"