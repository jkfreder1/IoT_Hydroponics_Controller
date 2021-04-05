function logoutButton() {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/logout", true);
  xhr.send();
  setTimeout(function(){ window.open("/logged-out","_self"); }, 1000);
}

function selectAllCheckboxes(section){
  
  checkboxes = document.getElementById(section).getElementsByClassName('checkboxContainer')[0].getElementsByTagName('input');
  for (var i = 0; i < checkboxes.length; i++) {
    checkboxes[i].checked = true;
  }
}

function clearAllCheckboxes(section){
  checkboxes = document.getElementById(section).getElementsByClassName('checkboxContainer')[0].getElementsByTagName('input')
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

function updateTablesDisplay() {
  checkboxes = document.getElementsByClassName('checkboxesTables');
  /*
  graphs = document.getElementsByClassName("graphElement");

  for(i=0; i<checkboxes.length; i++){
    if(checkboxes[i].checked == true)
    graphs[i].style.display = 'block';
    else
    graphs[i].style.display = 'none';

  }
  */
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



