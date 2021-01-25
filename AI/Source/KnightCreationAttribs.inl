namespace Obj{
	template <class T, typename Type>
	KnightCreationAttribs<T, Type>::KnightCreationAttribs(const int lvl, const int gridCols, const Sim* const sim):
		IEntityCreationAttribs<T, Type>()
	{
		IEntityCreationAttribs<T, Type>::im_Team = sim->turn == SimTurn::Player ? EntityTeam::Player : EntityTeam::AI;
		IEntityCreationAttribs<T, Type>::im_FacingDir = ChooseRandFacingDir();
		IEntityCreationAttribs<T, Type>::im_Lvl = lvl;

		IEntityCreationAttribs<T, Type>::im_LocalPos = FindValidLocalPos<T>(gridCols, sim);

		switch(lvl){
			case 1:
				IEntityCreationAttribs<T, Type>::im_ActionType = EntityActionType::UnidirectionalAttack;
				IEntityCreationAttribs<T, Type>::im_ActionRange = 1;
				IEntityCreationAttribs<T, Type>::im_VisionType = EntityVisionType::Omnidirectional;
				IEntityCreationAttribs<T, Type>::im_VisionRange = 1;

				IEntityCreationAttribs<T, Type>::im_ActionMag = 1.0f;
				IEntityCreationAttribs<T, Type>::im_Health = 4.0f;
				break;
			case 2:
				break;
			case 3:
				break;
		}
	}
}