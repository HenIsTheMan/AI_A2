template <class T>
Grid<T>::Grid():
	Grid(
		(T)0,
		(T)0,
		(T)0,
		0,
		0
	)
{
}

template <class T>
Grid<T>::Grid(T cellWidth, T cellHeight, T lineThickness, int rows, int cols):
	im_BlockData(),
	im_CellWidth(cellWidth),
	im_CellHeight(cellHeight),
	im_LineThickness(lineThickness),
	im_Rows(rows),
	im_Cols(cols)
{
	UpdateBlockData();
	UpdateEntityData();
}

template <class T>
int Grid<T>::CalcAmtOfHorizLines() const{
	return im_Rows + 1;
}

template <class T>
int Grid<T>::CalcAmtOfVertLines() const{
	return im_Cols + 1;
}

template <class T>
T Grid<T>::CalcWidth() const{
	return im_Cols * im_CellWidth + (T)CalcAmtOfVertLines() * im_LineThickness;
}

template <class T>
T Grid<T>::CalcHeight() const{
	return im_Rows * im_CellHeight + (T)CalcAmtOfHorizLines() * im_LineThickness;
}

template <class T>
std::vector<std::vector<bool>>& Grid<T>::RetrieveEntityData(){
	return im_EntityData;
}

template <class T>
const std::vector<std::vector<bool>>& Grid<T>::GetBlockData() const{
	return im_BlockData;
}

template <class T>
const std::vector<std::vector<bool>>& Grid<T>::GetEntityData() const{
	return im_EntityData;
}

template <class T>
T Grid<T>::GetCellWidth() const{
	return im_CellWidth;
}

template <class T>
T Grid<T>::GetCellHeight() const{
	return im_CellHeight;
}

template <class T>
T Grid<T>::GetLineThickness() const{
	return im_LineThickness;
}

template <class T>
int Grid<T>::GetRows() const{
	return im_Rows;
}

template <class T>
int Grid<T>::GetCols() const{
	return im_Cols;
}

template <class T>
void Grid<T>::SetBlockData(const bool blockData, const ptrdiff_t& row, const ptrdiff_t& col){
	im_BlockData[row][col] = blockData;
}

template <class T>
void Grid<T>::SetEntityData(const bool entityData, const ptrdiff_t& row, const ptrdiff_t& col){
	im_EntityData[row][col] = entityData;
}

template <class T>
void Grid<T>::SetCellWidth(T cellWidth){
	im_CellWidth = cellWidth;
}

template <class T>
void Grid<T>::SetCellHeight(T cellHeight){
	im_CellHeight = cellHeight;
}

template <class T>
void Grid<T>::SetLineThickness(T lineThickness){
	im_LineThickness = lineThickness;
}

template <class T>
void Grid<T>::SetRows(int rows){
	im_Rows = rows;
	UpdateBlockData();
	UpdateEntityData();
}

template <class T>
void Grid<T>::SetCols(int cols){
	im_Cols = cols;
	UpdateBlockData();
	UpdateEntityData();
}

template <class T>
void Grid<T>::UpdateBlockData(){
	std::vector<std::vector<bool>> oldBlockData = im_BlockData; //Make copy of data
	im_BlockData = std::vector<std::vector<bool>>(im_Rows);

	///Create vars
	int i, j;
	const size_t oldBlockDataSize = oldBlockData.size();

	for(i = 0; i < im_Rows; ++i){
		im_BlockData[i] = std::vector<bool>(im_Cols);
		for(j = 0; j < im_Cols; ++j){
			im_BlockData[i][j] = false; //All data becomes false
		}
	}

	for(i = 0; i < (im_Rows < (int)oldBlockDataSize ? im_Rows : (int)oldBlockDataSize); ++i){
		const size_t oldAmtOfCols = oldBlockData[i].size();
		for(j = 0; j < (im_Cols < (int)oldAmtOfCols ? im_Cols : (int)oldAmtOfCols); ++j){
			im_BlockData[i][j] = oldBlockData[i][j];
		}
	}
}

template <class T>
void Grid<T>::UpdateEntityData(){
	im_EntityData = std::vector<std::vector<bool>>(im_Rows);
	int i, j;

	for(i = 0; i < im_Rows; ++i){
		im_EntityData[i] = std::vector<bool>(im_Cols);
		for(j = 0; j < im_Cols; ++j){
			im_EntityData[i][j] = false; //All data becomes false
		}
	}
}