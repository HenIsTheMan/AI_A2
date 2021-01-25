namespace Obj{
	template <class T>
	T FindValidLocalPos(const Sim* const sim){
		assert(false);
		return T();
	}

	template <>
	Vector3 FindValidLocalPos(const Sim* const sim){
	}
}