template <class T>
HexGrid<T>::HexGrid():
	HexGrid(
		GridType::FlatTop,
		(T)0,
		(T)0,
		(T)0,
		0,
		0
	)
{
}

template <class T>
HexGrid<T>::HexGrid(const GridType type, const T cellScaleX, const T cellScaleY, const T lineThickness, const int rows, const int cols):
	im_Type(type),
	im_CellScaleX(cellScaleX),
	im_CellScaleY(cellScaleY),
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

template <class T>
T HexGrid<T>::CalcWidth() const{
	return im_Type == GridType::FlatTop
		? im_Cols * im_CellScaleX + (T)CalcAmtOfVertLines() * im_LineThickness
		: (T)im_Cols * ((T)CalcCellFlatToFlatLen() + im_LineThickness) + im_LineThickness;
}

template <class T>
T HexGrid<T>::CalcHeight() const{
	return im_Type == GridType::FlatTop
		? (T)im_Rows * ((T)CalcCellFlatToFlatLen() + im_LineThickness) + im_LineThickness
		: im_Rows * im_CellScaleY + (T)CalcAmtOfHorizLines() * im_LineThickness;
}

template <class T>
T HexGrid<T>::CalcCellSharpToSharpLen() const{
	return im_Type == GridType::FlatTop
		? im_CellScaleX
		: im_CellScaleY;
}

template <class T>
T HexGrid<T>::CalcCellFlatToFlatLen() const{
	return im_Type == GridType::FlatTop
		? im_CellScaleX * sinf(Math::DegreeToRadian(60)) //2.0f * im_CellScaleX * 0.5f * sinf(Math::DegreeToRadian(60))
		: im_CellScaleY * sinf(Math::DegreeToRadian(60)); //2.0f * im_CellScaleY * 0.5f * sinf(Math::DegreeToRadian(60))
}

template <class T>
T HexGrid<T>::CalcCellSideLen() const{
	return (im_Type == GridType::FlatTop
		? im_CellScaleX
		: im_CellScaleY) * cosf(Math::DegreeToRadian(60)); //...
}

template <class T>
T HexGrid<T>::CalcAltOffsetX() const{
	return im_Type == GridType::FlatTop
		? 0.0f
		: im_CellScaleX * 0.5f;
}

template <class T>
T HexGrid<T>::CalcAltOffsetY() const{
	return im_Type == GridType::FlatTop
		? im_CellScaleY * 0.5f
		: 0.0f;
}

template <class T>
typename HexGrid<T>::GridType HexGrid<T>::GetGridType() const{
	return im_Type;
}

template <class T>
T HexGrid<T>::GetCellScaleX() const{
	return im_CellScaleX;
}

template <class T>
T HexGrid<T>::GetCellScaleY() const{
	return im_CellScaleY;
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
void HexGrid<T>::SetGridType(const typename GridType type){
	im_Type = type;
}

template <class T>
void HexGrid<T>::SetCellScaleX(const T cellScaleX){
	im_CellScaleX = cellScaleX;
}

template <class T>
void HexGrid<T>::SetCellScaleY(const T cellScaleY){
	im_CellScaleY = cellScaleY;
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