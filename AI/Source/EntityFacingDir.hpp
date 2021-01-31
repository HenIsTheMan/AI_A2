#pragma once

namespace Obj{
	enum struct EntityFacingDir: int{
		Invalid,
		Up,
		Down,
		Left,
		Right,
		UL,
		UR,
		DL,
		DR,
		Amt
	};

	static EntityFacingDir entityFacingDirs[(int)EntityFacingDir::Amt]{
		EntityFacingDir::Invalid,
		EntityFacingDir::Up,
		EntityFacingDir::Down,
		EntityFacingDir::Left,
		EntityFacingDir::Right,
		EntityFacingDir::UL,
		EntityFacingDir::UR,
		EntityFacingDir::DL,
		EntityFacingDir::DR,
	};
}