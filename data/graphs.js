
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

   }, 200000 ) ;

window.onload = (event) => {
    secondaryInit(myChartAirTemp,'Air Temperature Live Data', 'Temperature (C)');
    secondaryInit(myChartAirTempDaily, 'Air Temperature Daily Averages', 'Temperature (C)');
    secondaryInit(myChartAirTempWeekly, 'Air Temperature Weekly Averages', 'Temperature (C)');
    secondaryInit(myChartAirTempMonthly, 'Air Temperature Monthly Averages', 'Temperature (C)');
    
    secondaryInit(myChartAirHumidity,'Air Humidity Live Data', '%');
    secondaryInit(myChartAirHumidityDaily,'Air Humidity Daily Averages', '%');
    secondaryInit(myChartAirHumidityWeekly,'Air Humidity Weekly Averages', '%');
    secondaryInit(myChartAirHumidityMonthly,'Air Humidity Monthly Averages', '%');
    

    
    hideHistoricals();

};