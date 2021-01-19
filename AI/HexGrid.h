#pragma once

template <class T>
class HexGrid final{
public:
	HexGrid<T>();
	HexGrid<T>(const T cellWidth, const T cellHeight, const T lineThickness, const int rows, const int cols);
	~HexGrid<T>() = default;

	int CalcAmtOfHorizLines() const;
	int CalcAmtOfVertLines() const;
	//T CalcWidth() const;
	//T CalcHeight() const;

	//* Getters
	T GetCellWidth() const;
	T GetCellHeight() const;
	T GetLineThickness() const;
	int GetRows() const;
	int GetCols() const;
	//*/

	//* Setters
	void SetCellWidth(const T cellWidth);
	void SetCellHeight(const T cellHeight);
	void SetLineThickness(const T lineThickness);
	void SetRows(const int rows);
	void SetCols(const int cols);
	//*/
private:
	T im_CellWidth;
	T im_CellHeight;
	T im_LineThickness;

	int im_Rows;
	int im_Cols;
};

#include "HexGrid.inl"