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
	HexGrid<T>(const GridType type, const T cellWidth, const T cellHeight, const T lineThickness, const int rows, const int cols);
	~HexGrid<T>() = default;

	int CalcAmtOfHorizLines() const;
	int CalcAmtOfVertLines() const;
	T CalcWidth() const;
	T CalcHeight() const;

	//* Getters
	GridType GetGridType() const;
	T GetCellWidth() const;
	T GetCellHeight() const;
	T GetLineThickness() const;
	int GetRows() const;
	int GetCols() const;
	//*/

	//* Setters
	void SetGridType(const GridType type);
	void SetCellWidth(const T cellWidth);
	void SetCellHeight(const T cellHeight);
	void SetLineThickness(const T lineThickness);
	void SetRows(const int rows);
	void SetCols(const int cols);
	//*/
private:
	GridType im_Type;

	T im_CellWidth;
	T im_CellHeight;
	T im_LineThickness;

	int im_Rows;
	int im_Cols;
};

#include "HexGrid.inl"