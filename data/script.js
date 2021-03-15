function logoutButton() {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/logout", true);
  xhr.send();
  setTimeout(function(){ window.open("/logged-out","_self"); }, 1000);
}

function openTab(evt, tabName) {
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
  document.getElementById(tabName).style.display = "block";
  evt.currentTarget.className += " active";

  
}



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