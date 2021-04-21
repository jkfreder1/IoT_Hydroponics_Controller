var timestampsLive = [];
var timestampsDaily = [];
var timestampsWeekly = [];
var timestampsMonthly = [];


var defaultData = {

    //labels: [ '12PM' , '1AM', '2AM', '3AM', '4AM', '5AM', '6AM', '7AM', '8AM', '9AM', '10AM', '11AM', '12PM', '1PM', '2PM', '3PM', '4PM', '5PM', '6PM', '7PM', '8PM', '9PM', '10PM', '11PM'  ],
    labels: [],
    datasets: [{
        label: 'Submission',
        data: [ ],
        borderColor: ["rgba(90,150,200,.6)"],
        fill: false
    }]
}

var defaultDataDaily = {

    //labels: [ 'Apr 1' , 'Apr 2', 'Apr 3', 'Apr 4', 'Apr 5', 'Apr 6', 'Apr 7', 'Apr 8', 'Apr 9', 'Apr 10', 'Apr 11', 'Apr 12', 'Apr 13', 'Apr 14', 'Apr 15', 'Apr 16', 'Apr 17', 'Apr 18', 'Apr 19', 'Apr 20', 'Apr 21', 'Apr 22', 'Apr 23', 'Apr 24'  ],
    labels: [],

    datasets: [{
        label: 'Submission',
        data: [ ],
        borderColor: ["rgba(90,150,200,.6)"],
        fill: false
    }]
}

var defaultDataWeekly = {

    //labels: [ 'Apr 4 2021' , 'Apr 11 2021', 'Apr 18 2021', 'Apr 25 2021', 'May 2 2021', 'May 9 2021', 'May 16 2021', 'May 23 2021', 'May 30 2021'],
    labels: [],

    datasets: [{
        label: 'Submission',
        data: [ ],
        borderColor: ["rgba(90,150,200,.6)"],
        fill: false
    }]
}
var defaultDataMonthly = {

    //labels: [ 'Apr 2021' , 'May 2021', 'Jun 2021', 'Jul 2021', 'Aug 2021', 'Sep 2021', 'Oct 2021', 'Nov 2021', 'Dec 2021', 'Jan 2022', 'Feb 2022', 'Mar 2022'],
    labels: [],

    datasets: [{
        label: 'Submission',
        data: [ ],
        borderColor: ["rgba(90,150,200,.6)"],
        fill: false
    }]
}


var defaultOptions = {
    
    //maintainAspectRatio: false,
    title: {
        fontSize: 35,
        padding: 20,
        display: true,
        text: 'default_title'
    },
    legend: {
        display: false,
    },
    layout: {
        padding: {
           left: 75,
           right: 75,
           top: 50,
           bottom: 50,
       }
    },
    
    scales: {
        yAxes: [{
            scaleLabel: {
                display: true,
                labelString: 'defaultYTitle',
                fontSize: 25,
                padding: 10,
            },
            gridLines: {
                display: true,
                lineWidth: 2,
            },
            ticks: {
                min: 60,
                max: 90,
                stepSize: 2,
                beginAtZero: true,
                display: true,
                maxTicksLimit: 24,
            }
        }],
        xAxes: [{
            scaleLabel: {
                display: true,
                //labelString: 'Time',
                fontSize: 25,
                //padding: 10,
            },
                      
        }]
    },
    tooltips: {
        mode: 'index',
        intersect: false
     },
     hover: {
        mode: 'index',
        intersect: false
     }
}
function secondaryInit(chart,title,yAxis,ymin,ymax, ystep,xAxis) {
    chart.options.title.text = title;
    chart.options.scales.yAxes[0].scaleLabel.labelString = yAxis;
    chart.options.scales.yAxes[0].ticks.min = ymin;
    chart.options.scales.yAxes[0].ticks.max = ymax;
    chart.options.scales.yAxes[0].ticks.stepSize = ystep;

    chart.update();

}
var ctxairTemp = document.getElementById('airTemp').getContext('2d');
var myChartAirTemp = new Chart(ctxairTemp, {
    type: 'line',
    data: defaultData,
    options: defaultOptions,
    
});



var ctxairTempDaily = document.getElementById('airTempDaily').getContext('2d');
var myChartAirTempDaily = new Chart(ctxairTempDaily, {
    type: 'line',
    data: defaultDataDaily,
    options: defaultOptions,
    
});

var ctxairTempWeekly = document.getElementById('airTempWeekly').getContext('2d');
var myChartAirTempWeekly = new Chart(ctxairTempWeekly, {
    type: 'line',
    data: defaultDataWeekly,
    options: defaultOptions,
    
});

var ctxairTempMonthly = document.getElementById('airTempMonthly').getContext('2d');
var myChartAirTempMonthly = new Chart(ctxairTempMonthly, {
    type: 'line',
    data: defaultDataMonthly,
    options: defaultOptions,
    
});


var ctxairHumidity = document.getElementById('airHumidity').getContext('2d');
var myChartAirHumidity = new Chart(ctxairHumidity, {
    type: 'line',
    data: defaultData,
    options: defaultOptions,
    
});

var ctxairHumidityDaily = document.getElementById('airHumidityDaily').getContext('2d');
var myChartAirHumidityDaily = new Chart(ctxairHumidityDaily, {
    type: 'line',
    data: defaultDataDaily,
    options: defaultOptions,
    
});

var ctxairHumidityWeekly = document.getElementById('airHumidityWeekly').getContext('2d');
var myChartAirHumidityWeekly= new Chart(ctxairHumidityWeekly, {
    type: 'line',
    data: defaultDataWeekly,
    options: defaultOptions,
    
});

var ctxairHumidityMonthly = document.getElementById('airHumidityMonthly').getContext('2d');
var myChartAirHumidityMonthly= new Chart(ctxairHumidityMonthly, {
    type: 'line',
    data: defaultDataMonthly,
    options: defaultOptions,
    
});

var ctxwaterTemp = document.getElementById('waterTemp').getContext('2d');
var myChartWaterTemp = new Chart(ctxwaterTemp, {
    type: 'line',
    data: defaultData,
    options: defaultOptions,
    
});

var ctxwaterTempDaily = document.getElementById('waterTempDaily').getContext('2d');
var myChartWaterTempDaily = new Chart(ctxwaterTempDaily, {
    type: 'line',
    data: defaultDataDaily,
    options: defaultOptions,
    
});

var ctxwaterTempWeekly = document.getElementById('waterTempWeekly').getContext('2d');
var myChartWaterTempWeekly= new Chart(ctxwaterTempWeekly, {
    type: 'line',
    data: defaultDataWeekly,
    options: defaultOptions,
    
});

var ctxwaterTempMonthly = document.getElementById('waterTempMonthly').getContext('2d');
var myChartWaterTempMonthly= new Chart(ctxwaterTempMonthly, {
    type: 'line',
    data: defaultDataMonthly,
    options: defaultOptions,
    
});

var ctxwaterLvl = document.getElementById('waterLvl').getContext('2d');
var myChartWaterLvl = new Chart(ctxwaterLvl, {
    type: 'line',
    data: defaultData,
    options: defaultOptions,
    
});

var ctxwaterLvlDaily = document.getElementById('waterLvlDaily').getContext('2d');
var myChartWaterLvlDaily = new Chart(ctxwaterLvlDaily, {
    type: 'line',
    data: defaultDataDaily,
    options: defaultOptions,
    
});

var ctxwaterLvlWeekly = document.getElementById('waterLvlWeekly').getContext('2d');
var myChartWaterLvlWeekly= new Chart(ctxwaterLvlWeekly, {
    type: 'line',
    data: defaultDataWeekly,
    options: defaultOptions,
    
});

var ctxwaterLvlMonthly = document.getElementById('waterLvlMonthly').getContext('2d');
var myChartWaterLvlMonthly= new Chart(ctxwaterLvlMonthly, {
    type: 'line',
    data: defaultDataMonthly,
    options: defaultOptions,
    
});

var ctxpH = document.getElementById('pH').getContext('2d');
var myChartpH = new Chart(ctxpH, {
    type: 'line',
    data: defaultData,
    options: defaultOptions,
    
});

var ctxpHDaily = document.getElementById('pHDaily').getContext('2d');
var myChartpHDaily = new Chart(ctxpHDaily, {
    type: 'line',
    data: defaultDataDaily,
    options: defaultOptions,
    
});

var ctxpHWeekly = document.getElementById('pHWeekly').getContext('2d');
var myChartpHWeekly= new Chart(ctxpHWeekly, {
    type: 'line',
    data: defaultDataWeekly,
    options: defaultOptions,
    
});

var ctxpHMonthly = document.getElementById('pHMonthly').getContext('2d');
var myChartpHMonthly= new Chart(ctxpHMonthly, {
    type: 'line',
    data: defaultDataMonthly,
    options: defaultOptions,
    
});

var ctxnutrientLvl = document.getElementById('nutrientLvl').getContext('2d');
var myChartNutrientLvl = new Chart(ctxnutrientLvl, {
    type: 'line',
    data: defaultData,
    options: defaultOptions,
    
});

var ctxnutrientLvlDaily = document.getElementById('nutrientLvlDaily').getContext('2d');
var myChartNutrientLvlDaily = new Chart(ctxnutrientLvlDaily, {
    type: 'line',
    data: defaultDataDaily,
    options: defaultOptions,
    
});

var ctxnutrientLvlWeekly = document.getElementById('nutrientLvlWeekly').getContext('2d');
var myChartNutrientLvlWeekly= new Chart(ctxnutrientLvlWeekly, {
    type: 'line',
    data: defaultDataWeekly,
    options: defaultOptions,
    
});

var ctxnutrientLvlMonthly = document.getElementById('nutrientLvlMonthly').getContext('2d');
var myChartNutrientLvlMonthly= new Chart(ctxnutrientLvlMonthly, {
    type: 'line',
    data: defaultDataMonthly,
    options: defaultOptions,
    
});



function addGraphData(chart, data, timestamps) {

    chart.data.labels = timestamps[0].dataset;

    chart.data.datasets.forEach((dataset) => {
        dataset.data = data.dataset;
    });

    chart.update();
}

function JSON_request(jsonData,chart,table,timestamps,summaryTable){
    let xhttpL = new XMLHttpRequest();
    xhttpL.onreadystatechange = function() {
     if (this.readyState == 4 && this.status == 200) {
        var myArr = JSON.parse(this.responseText);
        addGraphData(chart,myArr,timestamps);
        addTableData(table,myArr,timestamps);
        if(summaryTable)
            addSummary(summaryTable,myArr,timestamps);
      }
    };
    xhttpL.ontimeout = function (e) {
        console.log('timeout occurred for ${jsonData}');
      };
    xhttpL.open("GET", jsonData, true);
    xhttpL.timeout = 5000;
    xhttpL.send();
}
function JSON_requestTime(jsonData,timeContainer){
    let xhttpL = new XMLHttpRequest();
    xhttpL.onreadystatechange = function() {
     if (this.readyState == 4 && this.status == 200) {
        timeContainer[0] = JSON.parse(this.responseText);
     }
    };
    xhttpL.ontimeout = function (e) {
        console.log('timeout occurred for ${jsonData}');
      };
    xhttpL.open("GET", jsonData, true);
    xhttpL.timeout = 5000;
    xhttpL.send();
}

function JSON_error(jsonData,errorContainer,errorID){
    let xhttpL = new XMLHttpRequest();
    xhttpL.onreadystatechange = function() {
     if (this.readyState == 4 && this.status == 200) {
        errorContainer[0] = JSON.parse(this.responseText);
        
        updateErrors(errorContainer,errorID);
     }
    };
    xhttpL.ontimeout = function (e) {
        console.log('timeout occurred for ${jsonData}');
      };
    xhttpL.open("GET", jsonData, true);
    xhttpL.timeout = 5000;
    xhttpL.send();
}

setInterval(function ( ){ 
    
    JSON_requestTime("/timeStamp.json",timestampsLive);
    JSON_request("/data1.json",myChartAirTemp,tableAirTemp,timestampsLive,tableAirTempSummary);
    JSON_request("/data2.json",myChartAirHumidity,tableAirHumidity,timestampsLive,tableAirHumidSummary);
    JSON_request("/data3.json",myChartWaterTemp,tableWaterTemp,timestampsLive,tableWaterTempSummary);
    JSON_request("/data4.json",myChartNutrientLvl,tableNutrientLvl,timestampsLive,tableNutrientLvlSummary);
    JSON_request("/data5.json",myChartpH,tablepH,timestampsLive,tablepHSummary);
    JSON_request("/data6.json",myChartWaterLvl,tableWaterLvl,timestampsLive,tableWaterLvlSummary);

    JSON_error("/airTempError",errorAir,"errorAirTemp");
    JSON_error("/waterTempError",errorWaterTemp,"errorWaterTemp");
    JSON_error("/waterLvlError",errorWaterLevel,"errorWaterLvl");
    JSON_error("/pHError",errorpH,"errorpH");
    JSON_error("/nutrientLvlError",errorTDS,"errorNutrientLvl");

    JSON_requestTime("/dailyTimeStamp.json",timestampsDaily);
    JSON_request("/daily1.json",myChartAirTempDaily,tableAirTempDaily,timestampsDaily);
    JSON_request("/daily2.json",myChartAirHumidityDaily,tableAirHumidityDaily,timestampsDaily);
    JSON_request("/daily3.json",myChartWaterTempDaily,tableWaterTempDaily,timestampsDaily);
    JSON_request("/daily4.json",myChartNutrientLvlDaily,tableNutrientLvlDaily,timestampsDaily);
    JSON_request("/daily5.json",myChartpHDaily,tablepHDaily,timestampsDaily);
    JSON_request("/daily6.json",myChartWaterLvlDaily,tableWaterLvlDaily,timestampsDaily);

   }, 10000 ) ; 

   setInterval(function ( ){
    
    

    }, 105000 ) ;

    setInterval(function ( ){
        JSON_requestTime("/weeklyTimeStamp.json",timestampsWeekly);
        JSON_request("/weekly1.json",myChartAirTempWeekly,tableAirTempWeekly,timestampsWeekly);
        JSON_request("/weekly2.json",myChartAirHumidityWeekly,tableAirHumidityWeekly,timestampsWeekly);
        JSON_request("/weekly3.json",myChartWaterTempWeekly,tableWaterTempWeekly,timestampsWeekly);
        JSON_request("/weekly4.json",myChartNutrientLvlWeekly,tableNutrientLvlWeekly,timestampsWeekly);
        JSON_request("/weekly5.json",myChartpHWeekly,tablepHWeekly,timestampsWeekly);
        JSON_request("/weekly6.json",myChartWaterLvlWeekly,tableWaterLvlWeekly,timestampsWeekly);
    }, 350000 ) ;

    setInterval(function ( ){
        JSON_requestTime("/monthlyTimeStamp.json",timestampsMonthly);
        JSON_request("/monthly1.json",myChartAirTempMonthly,tableAirTempMonthly,timestampsMonthly);
        JSON_request("/monthly2.json",myChartAirHumidityMonthly,tableAirHumidityMonthly,timestampsMonthly);
        JSON_request("/monthly3.json",myChartWaterTempMonthly,tableWaterTempMonthly,timestampsMonthly);
        JSON_request("/monthly4.json",myChartNutrientLvlMonthly,tableNutrientLvlMonthly,timestampsMonthly);
        JSON_request("/monthly5.json",myChartpHMonthly,tablepHMonthly,timestampsMonthly);
        JSON_request("/monthly6.json",myChartWaterLvlMonthly,tableWaterLvlMonthly,timestampsMonthly);
    }, 1000000 ) ;


window.onload = (event) => {
    secondaryInit(myChartAirTemp,'Air Temperature Live Data', 'Temperature (F)', 60, 100,5);
    secondaryInit(myChartAirTempDaily, 'Air Temperature Daily Averages', 'Temperature (F)', 60, 100,5);
    secondaryInit(myChartAirTempWeekly, 'Air Temperature Weekly Averages', 'Temperature (F)', 60, 100,5);
    secondaryInit(myChartAirTempMonthly, 'Air Temperature Monthly Averages', 'Temperature (F)', 60, 100,5);
    
    secondaryInit(myChartAirHumidity,'Air Humidity Live Data', '%', 30, 80,5 );
    secondaryInit(myChartAirHumidityDaily,'Air Humidity Daily Averages', '%', 30, 80,5);
    secondaryInit(myChartAirHumidityWeekly,'Air Humidity Weekly Averages', '%', 30, 80,5);
    secondaryInit(myChartAirHumidityMonthly,'Air Humidity Monthly Averages', '%', 30, 80,5);

    secondaryInit(myChartWaterTemp,'Water Temperature Live Data', 'Temperature (F)', 60, 100,5);
    secondaryInit(myChartWaterTempDaily,'Water Temperature Daily Averages', 'Temperature (F)', 60, 100,5);
    secondaryInit(myChartWaterTempWeekly,'Water Temperature Weekly Averages', 'Temperature (F)', 60, 100,5);
    secondaryInit(myChartWaterTempMonthly,'Water Temperature Monthly Averages', 'Temperature (F)', 60, 100,5);

    secondaryInit(myChartWaterLvl,'Water Level Live Data', 'Distance from Water Surface (inches)',0,10,.5);
    secondaryInit(myChartWaterLvlDaily,'Water Level Daily Averages', 'Distance from Water Surface (inches)',0,10,.5);
    secondaryInit(myChartWaterLvlWeekly,'Water Level Weekly Averages', 'Distance from Water Surface (inches)',0,10,.5);
    secondaryInit(myChartWaterLvlMonthly,'Water Level Monthly Averages', '%Distance from Water Surface (inches)',0,10,.5);

    secondaryInit(myChartpH,'pH Live Data', 'pH',0,14,.5);
    secondaryInit(myChartpHDaily,'pH Daily Averages', 'pH',0,14,.5);
    secondaryInit(myChartpHWeekly,'pH Weekly Averages', 'pH',0,14,.5);
    secondaryInit(myChartpHMonthly,'pH Monthly Averages', 'pH',0,14,.5);

    secondaryInit(myChartNutrientLvl,'Nutrient Level Live Data', 'Parts Per Million (ppm)', 0, 1000, 25);
    secondaryInit(myChartNutrientLvlDaily,'Nutrient Level Daily Averages', 'Parts Per Million (ppm)', 0, 1000, 25);
    secondaryInit(myChartNutrientLvlWeekly,'Nutrient Level Weekly Averages', 'Parts Per Million (ppm)', 0, 1000, 25);
    secondaryInit(myChartNutrientLvlMonthly,'Nutrient Level Monthly Averages', 'Parts Per Million (ppm)', 0, 1000, 25);
    

    
    hideHistoricals();
    showActiveTab();
    /*
    table.redraw();

    addTableData(tableData,test);
    table.redraw(); */
};