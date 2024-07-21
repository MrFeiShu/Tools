Page({

  /**
   * 页面的初始数据
   */
  data: {
    matrix: [], // 二维数组，用于显示页面的文字。更新该数组，即可使页面文字更新
    currentPage: 0, // 当前页码
    rowsPerPage: 0, // 每页显示的行数
    columnsPerPage: 0, // 每页显示的列数
    totalPages: 0, // 总页数
    charWidthPx: 50, // 每个字的宽度，像素
    charHeightPx: 50, // 每个字的高度，像素
    totalCharNum: 0, // 每页显示的字符总数
    originalString: "人之初，性本善。性相近，习相远。苟不教，性乃迁。教之道，贵以专。昔孟母，择邻处。子不学，断机杼。窦燕山，有义方。教五子，名俱扬。养不教，父之过。教不严，师之惰。子不学，非所宜。幼不学，老何为。玉不琢，不成器。人不学，不知义。为人子，方少时。亲师友，习礼仪。",
    charArray: [], // 存放没有标点符号的中文文字数组
    punctArray: [], // 专门存放标点符号的数组
    disItemsArray: [],  // 对象数组，用于显示
    touchStartX: 0,
    touchEndX: 0
  },

displayChars: function(){
  const curPageIndex = this.data.currentPage;
  const totalCharNumTmp = this.data.totalCharNum;
  const columnsPerPageTmp = this.data.columnsPerPage;
  const rowsPerPageTmp = this.data.rowsPerPage;
  const itemLength = this.data.disItemsArray.length;
  let matrixTmp = [];
  let startPos = 0;
  let endPos = 0;
  let disItemsTmp = [];

  // 取当前页面应该显示的字和标点符号
  startPos = curPageIndex * totalCharNumTmp;
  endPos = Math.min(itemLength, startPos + totalCharNumTmp);
  disItemsTmp = this.data.disItemsArray.slice(startPos, endPos);
  console.log(disItemsTmp);

  // 显示字符和标点符号
  matrixTmp = Array.from({length: columnsPerPageTmp}, (_, rowIndex) =>
      Array.from({length: rowsPerPageTmp}, (_, colIndex) =>
      disItemsTmp[rowIndex * rowsPerPageTmp + colIndex]
        )
      );

  this.setData({
    matrix: matrixTmp
  });
},

  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {
    console.log("onload enter.");
    
    // 计算每页可显示的字数
    const info = wx.getWindowInfo();
    console.log("info.screenHeight:"+info.screenHeight+"; info.screenWidth:"+info.screenWidth);
    console.log("info.windowHeight:"+info.windowHeight+"; info.windowWidth:"+info.windowWidth);

    this.setData({
      columnsPerPage: Math.floor(info.windowWidth / this.data.charWidthPx),
      rowsPerPage: Math.floor(info.windowHeight / this.data.charHeightPx) - 1
    });

    console.log("rowsPerPage:"+this.data.rowsPerPage+"; columnPerPage:"+this.data.columnsPerPage);

    // 设置每页显示的字符总数
    this.setData({
      totalCharNum: this.data.rowsPerPage * this.data.columnsPerPage
    });
    console.log("totalCharNum:"+this.data.totalCharNum);

    // 分离原文的中文文字和标点符号
    let charArrayTmp = [];
    let punctArrayTmp = [];
    for (let i = 0; i < this.data.originalString.length; i++) {
      const char = this.data.originalString[i];

      // 中文标点
      if (char == '，' || char == '。') {
        punctArrayTmp.pop();
        punctArrayTmp.push(char);
      }
      else{
        charArrayTmp.push(char); 
        punctArrayTmp.push(" ");
      }
    }
    this.setData({
      punctArray:punctArrayTmp,
      charArray:charArrayTmp
    });
    
    console.log("this.data.punct: ", this.data.punctArray);
    console.log("this.data.charString: ", this.data.charArray);

    // 将中文文字数组和标点符号数组转化为对象数组
    this.setData({
      disItemsArray: charArrayTmp.map((str1, index) => ({
        charA: str1,
        charB: punctArrayTmp[index]
      }))
    });
    console.log("print disItems begin.");
    console.log(this.data.disItemsArray);
    console.log("print disItems end.");

    // 初始化总页数
    this.setData({
      totalPages: Math.ceil(this.data.originalString.length / this.data.totalCharNum)
    });
    console.log("totalPages:"+this.data.totalPages);

    // 显示第一页
    this.setData({
      currentPage: 0
    });
    
    this.displayChars();
  },

  onNextPage: function () {
    console.log("onNextPage enter.");

    let currentPage = this.data.currentPage;
    currentPage++;
    console.log("currentPage: "+ currentPage);

    // 修改用于显示文字的数据
    this.setData({
      currentPage: currentPage
    });
    this.displayChars();
  },

  onPreviousPage: function () {
    console.log("onPreviousPage enter.");

    let currentPage = this.data.currentPage;
    currentPage--;
    console.log("currentPage: "+ currentPage);

    // 修改用于显示文字的数据
    this.setData({
      currentPage: currentPage
    });
    this.displayChars();
  },

  onCellClick: function(event) {
    console.log("onCellClick enter.");
    let index = 0;
    const rowsPerPageTmp = this.data.rowsPerPage;

    const rowIndexTmp = event.currentTarget.dataset.rowIndex;
    const colIndexTmp = event.currentTarget.dataset.colIndex;
    index = rowIndexTmp * rowsPerPageTmp + colIndexTmp;

    console.log("Clicked cell at row: " + rowIndexTmp + ", column: " + colIndexTmp + ", index: " + index);

    let matrixTmp = this.data.matrix;
    console.log(this.data.matrix);

    let charB = matrixTmp[rowIndexTmp][colIndexTmp].charB;
    console.log("origin char: " + matrixTmp[rowIndexTmp][colIndexTmp].charB);

    if(" " == charB){
      console.log("get space");
      charB = "，";
    }
    else if("，" == charB){
      console.log("get ，");
      charB = "。";
    }
    else if("。" == charB){
      console.log("get 。");
      charB = " ";
    }
    else{
      console.log("not match.");
    }
    
    console.log("new char: " + charB);
    matrixTmp[rowIndexTmp][colIndexTmp].charB = charB;

    this.setData({
      matrix: matrixTmp
    });
  },

  handleTouchStart: function(event) {
    this.setData({
      touchStartX: event.touches[0].clientX
    });
  },

  handleTouchMove: function(event) {
    // 不需要做任何事情，但我们保留这个函数以便将来可能的扩展
  },

  handleTouchEnd: function(event) {
    this.setData({
      touchEndX: event.changedTouches[0].clientX
    });

    const deltaX = this.data.touchStartX - this.data.touchEndX;
    const currentPage = this.data.currentPage;
    const totalPages = this.data.totalPages;

    if (deltaX > 50 && currentPage < totalPages - 1) {
      // 用户向左滑动，切换到下一页
      console.log("nextpage.");
      this.onNextPage();
    } else if (deltaX < -50 && currentPage > 0) {
      // 用户向右滑动，切换到上一页
      console.log("Previouspage.");
      this.onPreviousPage();
    }
  },

  /**
   * 生命周期函数--监听页面初次渲染完成
   */
  onReady: function () {
    
  },

  /**
   * 生命周期函数--监听页面显示
   */
  onShow: function () {
    
  },

  /**
   * 生命周期函数--监听页面隐藏
   */
  onHide: function () {
    
  },

  /**
   * 生命周期函数--监听页面卸载
   */
  onUnload: function () {
    
  },

  /**
   * 页面相关事件处理函数--监听用户下拉动作
   */
  onPullDownRefresh: function () {
    
  },

  /**
   * 页面上拉触底事件的处理函数
   */
  onReachBottom: function () {
    
  },

  /**
   * 用户点击右上角分享
   */
  onShareAppMessage: function () {
    
  }
})