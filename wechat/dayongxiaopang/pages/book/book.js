// pages/book/book.js
Page({

  /**
   * 页面的初始数据
   */
  data: {
    urlBook: "http://106.53.57.234:8080/book?", // http://106.53.57.234:8080/book?book="史记"&chapter="五帝本纪"
    matrix: [], // 二维数组，用于显示页面的文字。更新该数组，即可使页面文字更新
    currentPage: 0, // 当前页码
    rowsPerPage: 0, // 每页显示的行数
    columnsPerPage: 0, // 每页显示的列数
    totalPages: 0, // 总页数
    charWidthPx: 50, // 每个字的宽度，像素
    charHeightPx: 50, // 每个字的高度，像素
    totalCharNum: 0, // 每页显示的字符总数
    originalString: "人之初，性本善。性相近，习相远。苟不教，性乃迁。教之道，贵以专。昔孟母，择邻处。子不学，断机杼。窦燕山，有义方。教五子，名俱扬。养不教，父之过。教不严，师之惰。子不学，非所宜。幼不学，老何为。玉不琢，不成器。人不学，不知义。为人子，方少时。亲师友，习礼仪。",
    itemsArray: [], // 对象数组，保存标注模式下用于显示的文字和标点符号
    // 对象数组，审阅模式数据，包含文字、新标点符号、旧标点符号和样式名称
    // 文字：即文本文字
    // 新标点符号：即用户标记的标点符号
    // 旧标点符号：即文本原始的标点符号；
    // 样式名称：即审阅时对于错误的标注需要使用明显的颜色标记出来
    touchStartX: 0,
    touchEndX: 0,
    displayMode: 0,  // 显示模式，0：标注模式(仅显示文字，不显示标点符号)，1：审阅模式(显示所有正确的标点符号以及错误的标点符号)
    totalPunctNum: 0,  // 总的标点符号数量
    urlShiyi: "http://106.53.57.234:8080/shiyi?wd=",
    shiyi: []
  },

displayChars: function(){
  const curPageIndex = this.data.currentPage;
  const totalCharNumTmp = this.data.totalCharNum;
  const columnsPerPageTmp = this.data.columnsPerPage;
  const rowsPerPageTmp = this.data.rowsPerPage;
  const itemLength = this.data.itemsArray.length;
  let matrixTmp = [];
  let startPos = 0;
  let endPos = 0;
  let disItemsTmp = [];

  // 取当前页面应该显示的字和标点符号
  startPos = curPageIndex * totalCharNumTmp;
  endPos = Math.min(itemLength, startPos + totalCharNumTmp);
  disItemsTmp = this.data.itemsArray.slice(startPos, endPos);
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

  initBook: function() {
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
    let originPunctArray = []; // 专门存放原始的标点符号的数组
    let newPunctArray = []; // 专门存放用户标注的标点符号的数组
    let charArray = []; // 存放没有标点符号的中文文字数组
    let cssArray = []; // 存放样式名称
    let checkPunctArray = []; // 专门存放审阅模式下的标点符号
    let totalPunctNumTmp = 0;

    for (let i = 0; i < this.data.originalString.length; i++) {
      const char = this.data.originalString[i];

      // 中文标点
      if (char == '，' || char == '。') {
        originPunctArray.pop();
        originPunctArray.push(char);
        cssArray.pop();
        cssArray.push("punct_error");
        checkPunctArray.pop();
        let checkPunctTmp = " ->" + char;
        checkPunctArray.push(checkPunctTmp);
        totalPunctNumTmp++;
      }
      else{
        charArray.push(char); 
        originPunctArray.push(" ");
        cssArray.push("punct_correct");
        newPunctArray.push(" "); // 用户标注的符号，默认使用空格，即表示没有符号
        checkPunctArray.push(" ");
      }
    }

    // 将中文文字数组和标点符号数组转化为对象数组
    // 初始化审阅模式数据
    this.setData({
      itemsArray: charArray.map((char, index) => ({
        hanzi: char,
        originPunct: originPunctArray[index],
        newPunct: newPunctArray[index],
        css: cssArray[index],
        checkPunct: checkPunctArray[index]
      })),
      totalPunctNum: totalPunctNumTmp
    });
    console.log("print itemsArray begin.");
    console.log(this.data.itemsArray);
    console.log("print itemsArray end.");

    // 初始化总页数
    this.setData({
      totalPages: Math.ceil(this.data.originalString.length / this.data.totalCharNum)
    });
    console.log("totalPages:"+this.data.totalPages);

    // 设置显示模式为标注模式
    this.setData({
      displayMode: 0
    });

    // 显示第一页
    this.setData({
      currentPage: 0
    });

    this.displayChars();
  },

  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {
    console.log("onload enter.");
    console.log(options.book); // 输出: value1  
    console.log(options.chapter); // 输出: value2 
    
    let urlTmp = this.data.urlBook;
    urlTmp += "book=";
    urlTmp += options.book;
    urlTmp += "&chapter=";
    urlTmp += options.chapter;

    console.log("url: " + urlTmp);

    const that = this;
    wx.request({
      url: urlTmp, // 服务器接口地址
      method: 'GET',
      data: {
        // GET请求参数，字符串键值对
      },
      header: {
        'content-type': 'application/json' // 默认值
      },
      success: function(res) {
        if (res.statusCode === 200) {
          console.log(res.data);
          
          that.setData({
            originalString: res.data
          });

          that.initBook();

        } else {
          console.error('请求失败，状态码：' + res.statusCode);
        }
      },
      fail: function() {
        console.error('请求失败');
      },
      complete: function() {
        // 请求完成后的回调函数
      }
    });
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

  onModifyPunctAndState: function(index, newPunct) {
    console.log("onModifyPunctAndState param: index: " + index + ", punct: " + newPunct);

    const currentPageTmp = this.data.currentPage;
    const totalCharNumTmp = this.data.totalCharNum;
    let itemsArrayTmp = this.data.itemsArray;

    let position = currentPageTmp * totalCharNumTmp + index;
    console.log("position: " + position);

    // 保存用户修改的标点符号
    itemsArrayTmp[position].newPunct = newPunct;

    // 修改审阅模式的相关数据
    if (itemsArrayTmp[position].newPunct == itemsArrayTmp[position].originPunct) {
      // 相等，说明用户标注正确
      itemsArrayTmp[position].checkPunct = itemsArrayTmp[position].originPunct;
      itemsArrayTmp[position].css = "punct_correct";
      console.log("modify punct success, checkPunct:" + itemsArrayTmp[position].checkPunct);
    }
    else{
      // 不相等，说明用户标注失败
      itemsArrayTmp[position].checkPunct = itemsArrayTmp[position].newPunct + "->" + itemsArrayTmp[position].originPunct;
      itemsArrayTmp[position].css = "punct_error";
      console.log("modify punct fail, checkPunct: " + itemsArrayTmp[position].checkPunct);
    }

    console.log(itemsArrayTmp);
    // 保存修改的内容
    this.setData({
      itemsArray: itemsArrayTmp
    });
  },

  onCellClick: function(event) {
    console.log("onCellClick enter.");

    const displayModeTmp = this.data.displayMode;
    if (1 == displayModeTmp) {
      console.log("check mode can not modify punct.");
      wx.showModal({
        title: '提示',
        content: '审阅模式下无法执行标注操作，请切换到标注模式。',
        showCancel: false, // 关闭取消按钮
        confirmText: '知道了', // 自定义确定按钮的文字
        success(res) {
          if (res.confirm) {
            console.log('用户点击确定')
          }
        }
      });
      return;
    }

    let index = 0;
    const rowsPerPageTmp = this.data.rowsPerPage;

    const rowIndexTmp = event.currentTarget.dataset.rowIndex;
    const colIndexTmp = event.currentTarget.dataset.colIndex;
    index = rowIndexTmp * rowsPerPageTmp + colIndexTmp;

    console.log("Clicked cell at row: " + rowIndexTmp + ", column: " + colIndexTmp + ", index: " + index);

    let matrixTmp = this.data.matrix;
    console.log(this.data.matrix);

    let newPunct = matrixTmp[rowIndexTmp][colIndexTmp].newPunct;
    console.log("origin char: " + matrixTmp[rowIndexTmp][colIndexTmp].newPunct);

    if(" " == newPunct){
      console.log("get space");
      newPunct = "，";
    }
    else if("，" == newPunct){
      console.log("get ，");
      newPunct = "。";
    }
    else if("。" == newPunct){
      console.log("get 。");
      newPunct = " ";
    }
    else{
      console.log("not match.");
    }
    
    console.log("new char: " + newPunct);
    matrixTmp[rowIndexTmp][colIndexTmp].newPunct = newPunct;

    this.onModifyPunctAndState(index, newPunct);

    this.setData({
      matrix: matrixTmp
    });
  },
  handleLongPress: function(event) {
    console.log('长按事件被触发');
    // 在这里处理长按事件，例如显示一个操作菜单
    let index = 0;
    const matrixTmp = this.data.matrix;
    const rowsPerPageTmp = this.data.rowsPerPage;

    const rowIndexTmp = event.currentTarget.dataset.rowIndex;
    const colIndexTmp = event.currentTarget.dataset.colIndex;
    index = rowIndexTmp * rowsPerPageTmp + colIndexTmp;

    console.log("Clicked cell at row: " + rowIndexTmp + ", column: " + colIndexTmp + ", index: " + index + " , hanzi: " + matrixTmp[rowIndexTmp][colIndexTmp].hanzi);
    console.log(matrixTmp);

    // 弹框显示该汉字的解释
    let url = this.data.urlShiyi;
    url += matrixTmp[rowIndexTmp][colIndexTmp].hanzi;
    console.log("url: " + url);
    
    const that = this;

    wx.request({
      url: url, // 服务器接口地址
      method: 'GET',
      data: {
        // GET请求参数，字符串键值对
      },
      header: {
        'content-type': 'application/json' // 默认值
      },
      success: function(res) {
        if (res.statusCode === 200) {
          let title = matrixTmp[rowIndexTmp][colIndexTmp].hanzi + "的释义";
          let shiyi = [];
          shiyi = res.data;
          console.log("shiyi" + shiyi);
          
          wx.showModal({
            title: title,
            content: shiyi,
            showCancel: false, // 关闭取消按钮
            confirmText: '知道了', // 自定义确定按钮的文字
            success(res) {
              if (res.confirm) {
                console.log('用户点击确定')
              }
            }
          });
        } else {
          console.error('请求失败，状态码：' + res.statusCode);
        }
      },
      fail: function() {
        console.error('请求失败');
      },
      complete: function() {
        // 请求完成后的回调函数
      }
    });
  },
  // 审阅标点符号是否正确标注。从第一页开始审阅
  onCheckAndMark: function(){
    let displayModeTmp = this.data.displayMode;
    let currentPageTmp = this.data.currentPage;

    if(1 == displayModeTmp){
      console.log("checkMode ---> viewMode.");
      displayModeTmp = 0;
    }
    else{      
      console.log("viewMode ---> checkMode.");
      displayModeTmp = 1;
    }

    if (1 == displayModeTmp) {
      // 如果切换到审阅模式，则先弹框提示标注结果
      // 结果要包含总的标点符号数量和标注正确的数量
      const totalPunctNumTmp = this.data.totalPunctNum;
      let correctPunctNum = totalPunctNumTmp;
      let resultMsg = "";
      let itemsArrayTmp = this.data.itemsArray;

      // 计算标注正确的标点符号数量
      for (let i = 0; i < itemsArrayTmp.length; i++) {
        if ("punct_error" == itemsArrayTmp[i].css) {
          correctPunctNum--;
        }
      }
      console.log("correctPunctNum: " + correctPunctNum + ", totalPunctNumTmp: " + totalPunctNumTmp);
      resultMsg = "您的标注结果：" + correctPunctNum + "/" + totalPunctNumTmp + "(正确数/总数)";
      console.log(resultMsg);

      wx.showModal({
        title: '提示',
        content: resultMsg,
        showCancel: false, // 关闭取消按钮
        confirmText: '知道了', // 自定义确定按钮的文字
        success(res) {
          if (res.confirm) {
            console.log('用户点击确定')
          }
        }
      });
    }

    currentPageTmp = 0;

    this.setData({
      displayMode: displayModeTmp,
      currentPage: currentPageTmp
    });

    this.displayChars();
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