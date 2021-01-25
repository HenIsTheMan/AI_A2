namespace Obj{
	template<class T, typename Type>
	KnightCreationAttribs<T, Type>::KnightCreationAttribs():
		IEntityCreationAttribs<T, Type>()
	{
		IEntityCreationAttribs<T, Type>::im_Team = EntityTeam::Player;
		IEntityCreationAttribs<T, Type>::im_FacingDir = EntityFacingDir::UL;
		IEntityCreationAttribs<T, Type>::im_Lvl = 1;

		IEntityCreationAttribs<T, Type>::im_LocalPos = T();

		IEntityCreationAttribs<T, Type>::im_Health = 5.0f;
	}
}