
var defaultData = {
    labels: [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24  ],
    datasets: [{
        label: 'Submission',
        data: [ ],
        borderColor: ["rgba(90,150,200,.6)"],
        fill: false
    }]
}

var defaultOptions = {
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
           left: 300,
           right: 100,
           top: 100,
           bottom: 100,
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
                labelString: 'Time (mm/dd/yyyy)',
                fontSize: 25,
                padding: 10,
            },
        }]

        
    }
}
function secondaryInit(chart,title,yAxis,xAxis) {
    chart.options.title.text = title;
    chart.options.scales.yAxes[0].scaleLabel.labelString = yAxis;
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
    data: defaultData,
    options: defaultOptions,
    
});

var ctxairTempWeekly = document.getElementById('airTempWeekly').getContext('2d');
var myChartAirTempWeekly = new Chart(ctxairTempWeekly, {
    type: 'line',
    data: defaultData,
    options: defaultOptions,
    
});

var ctxairTempMonthly = document.getElementById('airTempMonthly').getContext('2d');
var myChartAirTempMonthly = new Chart(ctxairTempMonthly, {
    type: 'line',
    data: defaultData,
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
    data: defaultData,
    options: defaultOptions,
    
});

var ctxairHumidityWeekly = document.getElementById('airHumidityWeekly').getContext('2d');
var myChartAirHumidityWeekly= new Chart(ctxairHumidityWeekly, {
    type: 'line',
    data: defaultData,
    options: defaultOptions,
    
});

var ctxairHumidityMonthly = document.getElementById('airHumidityMonthly').getContext('2d');
var myChartAirHumidityMonthly= new Chart(ctxairHumidityMonthly, {
    type: 'line',
    data: defaultData,
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
    data: defaultData,
    options: defaultOptions,
    
});

var ctxwaterTempWeekly = document.getElementById('waterTempWeekly').getContext('2d');
var myChartWaterTempWeekly= new Chart(ctxwaterTempWeekly, {
    type: 'line',
    data: defaultData,
    options: defaultOptions,
    
});

var ctxwaterTempMonthly = document.getElementById('waterTempMonthly').getContext('2d');
var myChartWaterTempMonthly= new Chart(ctxwaterTempMonthly, {
    type: 'line',
    data: defaultData,
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
    data: defaultData,
    options: defaultOptions,
    
});

var ctxwaterLvlWeekly = document.getElementById('waterLvlWeekly').getContext('2d');
var myChartWaterLvlWeekly= new Chart(ctxwaterLvlWeekly, {
    type: 'line',
    data: defaultData,
    options: defaultOptions,
    
});

var ctxwaterLvlMonthly = document.getElementById('waterLvlMonthly').getContext('2d');
var myChartWaterLvlMonthly= new Chart(ctxwaterLvlMonthly, {
    type: 'line',
    data: defaultData,
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
    data: defaultData,
    options: defaultOptions,
    
});

var ctxpHWeekly = document.getElementById('pHWeekly').getContext('2d');
var myChartpHWeekly= new Chart(ctxpHWeekly, {
    type: 'line',
    data: defaultData,
    options: defaultOptions,
    
});

var ctxpHMonthly = document.getElementById('pHMonthly').getContext('2d');
var myChartpHMonthly= new Chart(ctxpHMonthly, {
    type: 'line',
    data: defaultData,
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
    data: defaultData,
    options: defaultOptions,
    
});

var ctxnutrientLvlWeekly = document.getElementById('nutrientLvlWeekly').getContext('2d');
var myChartNutrientLvlWeekly= new Chart(ctxnutrientLvlWeekly, {
    type: 'line',
    data: defaultData,
    options: defaultOptions,
    
});

var ctxnutrientLvlMonthly = document.getElementById('nutrientLvlMonthly').getContext('2d');
var myChartNutrientLvlMonthly= new Chart(ctxnutrientLvlMonthly, {
    type: 'line',
    data: defaultData,
    options: defaultOptions,
    
});



function addData(chart, data) {

    chart.data.datasets.forEach((dataset) => {
        dataset.data = data.dataset;
    });

    chart.update();
}

function JSON_request(jsonData,chart){
    let xhttpL = new XMLHttpRequest();
    xhttpL.onreadystatechange = function() {
     if (this.readyState == 4 && this.status == 200) {
        var myArr = JSON.parse(this.responseText);
        addData(chart,myArr);
      }
    };
    xhttpL.open("GET", jsonData, true);
    xhttpL.send();
}


setInterval(function ( ){ 
    //JSON_request("/timestamps.json");
    JSON_request("/data1.json",myChartAirTemp);
    JSON_request("/data2.json",myChartAirHumidity);
    JSON_request("/data3.json",myChartWaterTemp);
    JSON_request("/data5.json",myChartpHDaily);
    JSON_request("/data6.json",myChartWaterLvl);

    JSON_request("/daily1.json",myChartAirTempDaily);
    JSON_request("/daily2.json",myChartAirHumidityDaily);
    JSON_request("/daily3.json",myChartWaterTempDaily);
    JSON_request("/daily5.json",myChartpHDaily);
    JSON_request("/daily6.json",myChartWaterLvlDaily);





   }, 20000 ) ;

window.onload = (event) => {
    secondaryInit(myChartAirTemp,'Air Temperature Live Data', 'Temperature (C)');
    secondaryInit(myChartAirTempDaily, 'Air Temperature Daily Averages', 'Temperature (C)');
    secondaryInit(myChartAirTempWeekly, 'Air Temperature Weekly Averages', 'Temperature (C)');
    secondaryInit(myChartAirTempMonthly, 'Air Temperature Monthly Averages', 'Temperature (C)');
    
    secondaryInit(myChartAirHumidity,'Air Humidity Live Data', '%');
    secondaryInit(myChartAirHumidityDaily,'Air Humidity Daily Averages', '%');
    secondaryInit(myChartAirHumidityWeekly,'Air Humidity Weekly Averages', '%');
    secondaryInit(myChartAirHumidityMonthly,'Air Humidity Monthly Averages', '%');

    secondaryInit(myChartWaterTemp,'Water Temperature Live Data', 'Temperature (C)');
    secondaryInit(myChartWaterTempDaily,'Water Temperature Daily Averages', 'Temperature (C)');
    secondaryInit(myChartWaterTempWeekly,'Water Temperature Weekly Averages', 'Temperature (C)');
    secondaryInit(myChartWaterTempMonthly,'Water Temperature Monthly Averages', 'Temperature (C)');

    secondaryInit(myChartWaterLvl,'Water Level Live Data', '%');
    secondaryInit(myChartWaterLvlDaily,'Water Level Daily Averages', '%');
    secondaryInit(myChartWaterLvlWeekly,'Water Level Weekly Averages', '%');
    secondaryInit(myChartWaterLvlMonthly,'Water Level Monthly Averages', '%');

    secondaryInit(myChartpH,'pH Live Data', '%');
    secondaryInit(myChartpHDaily,'pH Daily Averages', '%');
    secondaryInit(myChartpHWeekly,'pH Weekly Averages', '%');
    secondaryInit(myChartpHMonthly,'pH Monthly Averages', '%');
    

    
    hideHistoricals();

};