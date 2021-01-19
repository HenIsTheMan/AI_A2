template <class T>
HexGrid<T>::HexGrid():
	HexGrid(
		(T)0,
		(T)0,
		(T)0,
		0,
		0
	)
{
}

template <class T>
HexGrid<T>::HexGrid(const T cellWidth, const T cellHeight, const T lineThickness, const int rows, const int cols):
	im_CellWidth(cellWidth),
	im_CellHeight(cellHeight),
	im_LineThickness(lineThickness),
	im_Rows(rows),
	im_Cols(cols)
{
}

template <class T>
int HexGrid<T>::CalcAmtOfHorizLines() const{
	return im_Rows + 1;
}

template <class T>
int HexGrid<T>::CalcAmtOfVertLines() const{
	return im_Cols + 1;
}

//template <class T>
//T HexGrid<T>::CalcWidth() const{
//}
//
//template <class T>
//T HexGrid<T>::CalcHeight() const{
//}

template <class T>
T HexGrid<T>::GetCellWidth() const{
	return im_CellWidth;
}

template <class T>
T HexGrid<T>::GetCellHeight() const{
	return im_CellHeight;
}

template <class T>
T HexGrid<T>::GetLineThickness() const{
	return im_LineThickness;
}

template <class T>
int HexGrid<T>::GetRows() const{
	return im_Rows;
}

template <class T>
int HexGrid<T>::GetCols() const{
	return im_Cols;
}

template <class T>
void HexGrid<T>::SetCellWidth(const T cellWidth){
	im_CellWidth = cellWidth;
}

template <class T>
void HexGrid<T>::SetCellHeight(const T cellHeight){
	im_CellHeight = cellHeight;
}

template <class T>
void HexGrid<T>::SetLineThickness(const T lineThickness){
	im_LineThickness = lineThickness;
}

template <class T>
void HexGrid<T>::SetRows(const int rows){
	im_Rows = rows;
}

template <class T>
void HexGrid<T>::SetCols(const int cols){
	im_Cols = cols;
}