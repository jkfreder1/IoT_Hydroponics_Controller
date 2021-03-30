function logoutButton() {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/logout", true);
  xhr.send();
  setTimeout(function(){ window.open("/logged-out","_self"); }, 1000);
}

function selectAllCheckboxes(){
  
  checkboxes = document.getElementsByClassName('checkboxContainer')[0].getElementsByTagName('input');
  for (var i = 0; i < checkboxes.length; i++) {
    checkboxes[i].checked = true;
  }
}

function clearAllCheckboxes(){
  checkboxes = document.getElementsByClassName('checkboxContainer')[0].getElementsByTagName('input')
  for (var i = 0; i < checkboxes.length; i++) {
    checkboxes[i].checked = false;
  }
}


function selectLI(ulID){
  let ul = ulID.getElementsByTagName('input');
  for (let i = 1; i < ul.length; i++) {
    if(ul[0].checked == true)
       ul[i].checked = true;
    else
      ul[i].checked  = false;
    }

    
}

function updateGraphsDisplay() {
  checkboxes = document.getElementsByClassName('checkboxesGraphs');
  graphs = document.getElementsByClassName("graphElement");

  for(i=0; i<checkboxes.length; i++){
    if(checkboxes[i].checked == true)
    graphs[i].style.display = 'block';
    else
    graphs[i].style.display = 'none';

  }
  console.log(checkboxes);  
}

function showHistoricalGraph(evt,graphID,graphCategory) {
  let historyGraphs;
  console.log(graphID);
  console.log(graphCategory);
  
  // obtain historical graphs of category and hide all of them
  historyGraphs = document.getElementById(graphCategory).getElementsByTagName("canvas");
  console.log(historyGraphs);
  for (i = 0; i < historyGraphs.length; i++) {
    historyGraphs[i].style.display = "none";
  }

  // show the historical graph that was selected
  document.getElementById(graphID).style.display = "block";
  console.log(document.getElementById(graphID));

}

// function hides all historical graphs
function hideHistoricals(){
  // gather all historical containers that hold each of the historical categories
  let historyContainer = document.getElementsByClassName('historyGraphs');
  let historyGraphs;

  // for each historical container, hide the canvas elements (graphs) within it 
  for(i=0; i <historyContainer.length; i++){
    historyGraphs = historyContainer[i].getElementsByTagName("canvas");
    for (k = 0; k < historyGraphs.length; k++) {
      historyGraphs[k].style.display = "none";
    }
  }
  
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

  
}

window.onload = (event) => {
}

/*
function renderChart(tabName){

  // initialize variables 
  var chartTitleL = "blank";
  var yAxisL = "blank";
  var xAxisL = "time";
  var jsonFileL = "/test.json";
  var chartTitleH = "blank";
  var yAxisH = "blank";
  var xAxisH = "Day";
  var jsonFileH = "/test.json";
  
  // set variables to correct lables dependent on section the page is in
  switch(tabName) {
    case "section2":
      chartTitleL = "Live Air Temperature";
      yAxisL = "Fahrenheit";
      jsonFileL = "/data1.json";
      chartTitleH = "Daily Air Temperature Averages";
      yAxisH = "Fahrenheit";
      jsonFileH = "/daily1.json";
      break;
    case "section3":
      chartTitleL = "Live Air Humidity";
      yAxisL = "%";
      jsonFileL = "/data2.json";
      chartTitleH = "Daily Air Humidity Averages";
      yAxisH = "%";
      jsonFileH = "/daily2.json";
      break;
    default:
      // code block
  }
  
  // initialize data arrays
  var dataPointsL = [];
  var dataPointsH =  [];
  
  
  var chartL = new CanvasJS.Chart("chartContainerL", {
    height: 360,

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
    height: 360,

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

    dataPointsH.length = 0;
    for (var i = 0; i < data.dataset.length; i++) {
      dataPointsH.push({
        x: i,
        y: data.dataset[i],
      });
    }
    chartH.render();
   }
  
   setInterval(function ( ){ 
    let xhttpL = new XMLHttpRequest();
    xhttpL.onreadystatechange = function() {
     if (this.readyState == 4 && this.status == 200) {
        var myArrL = JSON.parse(this.responseText);
        addDataL(myArrL);
      }
    };
    xhttpL.open("GET", jsonFileL, true);
    xhttpL.send();
   }, 5000 ) ;
  
   setInterval(function ( ){ 

    let xhttpH = new XMLHttpRequest();
    xhttpH.onreadystatechange = function() {
     if (this.readyState == 4 && this.status == 200) {
        var myArrH = JSON.parse(this.responseText);
        addDataH(myArrH);
      }
    };
    xhttpH.open("GET", jsonFileH, true);
    if(dataPointsL.length>4)
      xhttpH.send();
   }, 5000 ) ;
   

  }
  
  
  
  
 
*/




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
     //addData(myArr);
   }
 };
 xhttp.open("GET", "/test.json", true);
 xhttp.send();
}, 800 ) ;
*/
