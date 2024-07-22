Page({

  /**
   * 页面的初始数据
   */
  data: {
    curBookIndex: 0,
    curChapterIndex: 0,
    urlCatalogue: "http://106.53.57.234:8080/catalogue",
    bookCatalogue: [], // 字符串数组，用于显示书籍目录
    chapterCatalogue: [], // 字符串数组，用于显示章节目录
    bookDataArray:[] // 对象数组，保存书籍所有目录信息
  },

  getBookData:function() {
    const urlTmp = this.data.urlCatalogue;

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
          // console.log(res.data);
          const jsonBooks = res.data;

          // 后面修改下json数据中的key值命名
          const bookDataArrayTmp = jsonBooks.map(book => ({
            book: book.name,
            chapters: book.chapter
          }));

          that.setData({
            bookDataArray: bookDataArrayTmp
          });

          that.disCatalogue();
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

  disCatalogue: function() {
    const bookDataArrayTmp = this.data.bookDataArray;
    let bookCatalogueTmp = this.data.bookCatalogue;
    let chapterCatalogueTmp = this.data.chapterCatalogue;
    let curBookIndexTmp = 0;
    let curChapterIndexTmp = 0;

    // 初始化书籍和章节目录
    for (let i = 0; i < bookDataArrayTmp.length; i++) {
      bookCatalogueTmp.push(bookDataArrayTmp[i].book);
      if ("史记" == bookDataArrayTmp[i].book) {
        // 初始化章节目录，默认显示史记
        chapterCatalogueTmp = bookDataArrayTmp[i].chapters;
      }
    }

    this.setData({
      bookCatalogue: bookCatalogueTmp,
      chapterCatalogue: chapterCatalogueTmp,
      curBookIndex: curBookIndexTmp,
      curChapterIndex: curChapterIndexTmp
    });
  },
  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {
    console.log("onload enter.");

    this.getBookData();
  },
  // 处理view2中子元素的点击事件  
  onBookTap: function(e) {  
    const index = e.currentTarget.dataset.index;  
    console.log(`View 2 Child ${index} was tapped`);  
    // 在这里可以添加更多的逻辑，比如跳转到另一个页面，显示弹窗等  
    let chapterCatalogueTmp = [];
    const bookDataArrayTmp = this.data.bookDataArray;

    chapterCatalogueTmp = bookDataArrayTmp[index].chapters;

    this.setData({
      chapterCatalogue: chapterCatalogueTmp,
      curBookIndex: index,
      curChapterIndex: 0
    });
  },  
    
  // 处理view3中子元素的点击事件  
  onChapterTap: function(e) {  
    const index = e.currentTarget.dataset.index;  
    console.log(`View 3 Child ${index} was tapped`);  
    // 同上，可以添加更多的逻辑  
    const bookDataArrayTmp = this.data.bookDataArray;
    const curBookIndexTmp = this.data.curBookIndex;

    this.setData({
      curChapterIndex: index
    });

    // 构造跳转URL，附带参数  
    let url = '/pages/book/book?book=';
    url += bookDataArrayTmp[curBookIndexTmp].book;
    url += "&chapter=";
    url += bookDataArrayTmp[curBookIndexTmp].chapters[index];
    console.log("url: " + url);

    // 跳转到book页面  
    wx.navigateTo({  
      url: url,  
    }); 
  } ,
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