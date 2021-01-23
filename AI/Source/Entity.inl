namespace Obj{
	template <class T, typename Type>
	Entity<T, Type>::Entity():
		Entity(EntityAttribs<T, Type>())
	{
	}

	template <class T, typename Type>
	Entity<T, Type>::Entity(const EntityAttribs<T, Type>& attribs):
		im_Attribs(attribs)
	{
	}

	template <class T, typename Type>
	int Entity<T, Type>::OnEvent(Event* myEvent, const bool destroyEvent){
		if(!myEvent){
			return -1;
		}

		int val = -1;

		switch(myEvent->GetID()){
		}

		if(destroyEvent && myEvent){
			delete myEvent;
			myEvent = nullptr;
		}
		return val;
	}
}