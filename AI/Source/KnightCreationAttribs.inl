namespace Obj{
	template <class T, typename Type>
	KnightCreationAttribs<T, Type>::KnightCreationAttribs(const int lvl, const int gridCols, const Sim* const sim):
		IEntityCreationAttribs<T, Type>()
	{
		IEntityCreationAttribs<T, Type>::im_Team = sim->turn == SimTurn::Player ? EntityTeam::Player : EntityTeam::AI;
		IEntityCreationAttribs<T, Type>::im_FacingDir = ChooseRandFacingDir();
		IEntityCreationAttribs<T, Type>::im_Lvl = lvl;

		IEntityCreationAttribs<T, Type>::im_LocalPos = FindValidLocalPos<T>(gridCols, sim);

		IEntityCreationAttribs<T, Type>::im_Health = 5.0f;
	}
}