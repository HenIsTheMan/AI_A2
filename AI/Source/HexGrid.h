#pragma once

template <class T>
class HexGrid final{
public:
	enum struct GridType: int{
		FlatTop,
		SharpTop,
		Amt
	};

	HexGrid<T>();
	HexGrid<T>(const GridType type, const T cellScaleX, const T cellScaleY, const T lineThickness, const int rows, const int cols);
	~HexGrid<T>() = default;

	int CalcAmtOfHorizLines() const;
	int CalcAmtOfVertLines() const;
	T CalcWidth() const;
	T CalcHeight() const;
	T CalcCellWidth() const;
	T CalcCellHeight() const;

	//* Getters
	GridType GetGridType() const;
	T GetCellScaleX() const;
	T GetCellScaleY() const;
	T GetLineThickness() const;
	int GetRows() const;
	int GetCols() const;
	//*/

	//* Setters
	void SetGridType(const GridType type);
	void SetCellScaleX(const T cellScaleX);
	void SetCellScaleY(const T cellScaleY);
	void SetLineThickness(const T lineThickness);
	void SetRows(const int rows);
	void SetCols(const int cols);
	//*/
private:
	GridType im_Type;

	T im_CellScaleX;
	T im_CellScaleY;
	T im_LineThickness; //Jagged

	int im_Rows; //Jagged if GridType::FlatTop
	int im_Cols; //Jagged if GridType::SharpTop
};

#include "HexGrid.inl"