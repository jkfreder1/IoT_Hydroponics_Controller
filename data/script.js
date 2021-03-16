function logoutButton() {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/logout", true);
  xhr.send();
  setTimeout(function(){ window.open("/logged-out","_self"); }, 1000);
}

function openTab(evt, tabSection, tabName) {
  // Declare all variables
  var i, tabcontent, tablinks;

  // Get all elements with class="tabcontent" and hide them
  tabcontent = document.getElementsByClassName("tabcontent");
  for (i = 0; i < tabcontent.length; i++) {
    tabcontent[i].style.display = "none";
  }

  // Get all elements with class="tabs" and remove the class "active"
  tablinks = document.getElementsByClassName("tabs");
  for (i = 0; i < tablinks.length; i++) {
    tablinks[i].className = tablinks[i].className.replace(" active", "");
  }

  // Show the current tab, and add an "active" class to the button that opened the tab
  document.getElementById(tabSection).style.display = "block";
  evt.currentTarget.className += " active";

  if(tabName == "section2" || tabName == "section3")
    renderChart(tabName);
}

function renderChart(tabName){

  // initialize variables 
  var chartTitleL = "blank";
  var yAxisL = "blank";
  var xAxisL = "time";
  var jsonFileL = "/test.json";
  var chartContainerL = "blank";
  var chartTitleH = "blank";
  var yAxisH = "blank";
  var xAxisH = "Day";
  var jsonFileH = "/test.json";
  var chartContainerH = "blank";

  // set variables to correct lables dependent on section the page is in
  switch(tabName) {
    case "section2":
      chartTitleL = "Live Air Temperature";
      yAxisL = "Fahrenheit";
      jsonFileL = "/data1.json";
      chartContainerL = "chartContainerL2";
      chartTitleH = "Daily Air Temperature Averages";
      yAxisH = "Fahrenheit";
      jsonFileH = "/daily1.json";
      chartContainerH = "chartContainerH2";
      break;
    case "section3":
      chartTitleL = "Live Air Humidity";
      yAxisL = "%";
      jsonFileL = "/data2.json";
      chartContainerL = "chartContainerL3";
      chartTitleH = "Daily Air Humidity Averages";
      yAxisH = "%";
      jsonFileH = "/daily2.json";
      chartContainerH = "chartContainerH2";
      break;
    default:
      // code block
  }

  // initialize data arrays
  var dataPointsL = [];
  var dataPointsH = [];


  var chartL = new CanvasJS.Chart("chartContainerL", {
    animationEnabled: true,
    theme: "light2",
    title: {
      text: chartTitleL
    },
    axisY: {
      title: yAxisL,
      titleFontSize: 24,
    },
    axisX: {
      title: xAxisL
    },
    data: [{
      type: "line",
      //yValueFormatString: "#,### Units",
      dataPoints: dataPointsL
    }]
   });


   var chartH = new CanvasJS.Chart("chartContainerH", {
    animationEnabled: true,
    theme: "light2",
    title: {
      text: chartTitleH
    },
    axisY: {
      title: yAxisH,
      titleFontSize: 24,
    },
    axisX: {
      title: xAxisH
    },
    data: [{
      type: "line",
      //yValueFormatString: "#,### Units",
      dataPoints: dataPointsH
    }]
   });


   function addDataL(data) {
    dataPointsL.length = 0;
    for (var i = 0; i < data.dataset.length; i++) {
      dataPointsL.push({
        x: i,
        y: data.dataset[i],
      });
    }
    chartL.render();
   }

   function addDataH(data) {
    dataPoints.length = 0;
    for (var i = 0; i < data.dataset.length; i++) {
      dataPointsH.push({
        x: i,
        y: data.dataset[i],
      });
    }
    chartH.render();
   }

   setInterval(function ( ){ 
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
     if (this.readyState == 4 && this.status == 200) {
        var myArr = JSON.parse(this.responseText);
        addDataL(myArr);
      }
    };
    xhttp.open("GET", jsonFileL, true);
    xhttp.send();
   }, 5000 ) ;

   setInterval(function ( ){ 
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
     if (this.readyState == 4 && this.status == 200) {
        var myArr = JSON.parse(this.responseText);
        addDataH(myArr);
      }
    };
    xhttp.open("GET", jsonFileH, true);
    xhttp.send();
   }, 5000 ) ;
}














/*
var tableData = [
  {id:1, name:"Billy Bob", age:"12", gender:"male", height:1, col:"red", dob:"", cheese: 1},
]

var table = new Tabulator("#example-table", {
  data:tableData, //set initial table data

  columns:[
      {title:"Name", field:"name"},
      {title:"Age", field:"age"},
      {title:"Gender", field:"gender"},
      {title:"Height", field:"height"},
      {title:"Favourite Color", field:"col"},
      {title:"Date Of Birth", field:"dob"},
      {title:"Cheese Preference", field:"cheese"},
  ],
});
*/
/*
window.onload = function() {
  var dataPoints = [];
  
  var chart = new CanvasJS.Chart("chartContainer", {
 animationEnabled: true,
 theme: "light2",
 title: {
   text: "Line Chart"
 },
 axisY: {
   title: "Measurement",
   titleFontSize: 24,
 },
 data: [{
   type: "line",
   //yValueFormatString: "#,### Units",
   dataPoints: dataPoints
 }]
});


function addData(data) {
 for (var i = 0; i < data.dataset.length; i++) {
   dataPoints.push({
     x: data.dataset[i],
     y: i,
   });
 }
 chart.render();

}

setInterval(function ( ){ 
 var xhttp = new XMLHttpRequest();
 xhttp.onreadystatechange = function() {
  if (this.readyState == 4 && this.status == 200) {
     var myArr = JSON.parse(this.responseText);
     addData(myArr);
   }
 };
 xhttp.open("GET", "/data.json", true);
 xhttp.send();
}, 800 ) ;
}
*/