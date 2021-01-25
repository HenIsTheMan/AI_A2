namespace Obj{
	template<class T, typename Type>
	KnightCreationAttribs<T, Type>::KnightCreationAttribs():
		EntityCreationAttribs<T, Type>()
	{
		EntityCreationAttribs<T, Type>::im_Team = EntityTeam::Player;
		EntityCreationAttribs<T, Type>::im_FacingDir = EntityFacingDir::UL;
		EntityCreationAttribs<T, Type>::im_Lvl = 1;

		EntityCreationAttribs<T, Type>::im_Health = 5.0f;
	}
}