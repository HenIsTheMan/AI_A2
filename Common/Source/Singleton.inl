template <class T>
T* Singleton<T>::objPtr = nullptr;

template <class T>
T* const Singleton<T>::RetrieveGlobalObjPtr(){
	if(!objPtr){ //Init on 1st use
		objPtr = new T();
	}
	return objPtr;
}

template <class T>
void Singleton<T>::Destroy(){
	if(objPtr){
		delete objPtr;
		objPtr = nullptr;
	}
}