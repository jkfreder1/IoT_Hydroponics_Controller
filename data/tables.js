

//define array of table data
var tableData = [
    {date: 1}
]
var test = [
    {}
]

//create table and assign data
var table = new Tabulator("#example-table", {
    reactiveData:true, //enable reactive data
    data:tableData, //assign data array
    layout:"fitDataTable",

    columns:[
          {title:"Date", field:"time"},
          {title:"Temperature", field:"dataset"},
          
    ]
});
                                    // **** SUMMARY TABLES **** //

var tableAirTempSummary = new Tabulator("#airTemp-summary", {
    reactiveData:true, //enable reactive data
    data:test, //assign data array
    layout:"fitDataTable",

    columns:[
          {title:"Date", field:"time"},
          {title:"Temperature", field:"dataset"},
          
    ]
});

var tableAirHumidSummary = new Tabulator("#airHumid-summary", {
    reactiveData:true, //enable reactive data
    data:test, //assign data array
    layout:"fitDataTable",

    columns:[
          {title:"Date", field:"time"},
          {title:"Temperature", field:"dataset"},
          
    ]
});

var tableWaterTempSummary = new Tabulator("#waterTemp-summary", {
    reactiveData:true, //enable reactive data
    data:test, //assign data array
    layout:"fitDataTable",
    columns:[
          {title:"Date", field:"time"},
          {title:"Temperature", field:"dataset"},
          
    ]
});

var tableWaterLvlSummary = new Tabulator("#waterLvl-summary", {
    reactiveData:true, //enable reactive data
    data:test, //assign data array
    layout:"fitDataTable",
    columns:[
          {title:"Date", field:"time"},
          {title:"Temperature", field:"dataset"},
          
    ]
});

var tablepHSummary = new Tabulator("#pH-summary", {
    reactiveData:true, //enable reactive data
    data:test, //assign data array
    layout:"fitDataTable",
    columns:[
          {title:"Date", field:"time"},
          {title:"Temperature", field:"dataset"},
          
    ]
});
var tableNutrientLvlSummary = new Tabulator("#nutrientLvl-summary", {
    reactiveData:true, //enable reactive data
    data:test, //assign data array
    layout:"fitDataTable",
    columns:[
          {title:"Date", field:"time"},
          {title:"Temperature", field:"dataset"},
          
    ]
});
                                    // **** DATA TABLES **** //

var tableAirTemp = new Tabulator("#airTempT", {
    reactiveData:true, //enable reactive data
    data:tableData, //assign data array
    layout:"fitDataTable",

    columns:[
          {title:"Date", field:"time"},
          {title:"Temperature", field:"dataset"},
          
    ]
});
var tableAirTempDaily = new Tabulator("#airTempDailyT", {
    reactiveData:true, //enable reactive data
    data:tableData, //assign data array
    layout:"fitDataTable",

    columns:[
          {title:"Date", field:"time"},
          {title:"Temperature", field:"dataset"},
          
    ]
});

var tableAirTempWeekly = new Tabulator("#airTempWeeklyT", {
    reactiveData:true, //enable reactive data
    data:tableData, //assign data array
    layout:"fitDataTable",

    columns:[
          {title:"Date", field:"time"},
          {title:"Temperature", field:"dataset"},
          
    ]
});

var tableAirTempMonthly = new Tabulator("#airTempMonthlyT", {
    reactiveData:true, //enable reactive data
    data:tableData, //assign data array
    layout:"fitDataTable",

    columns:[
          {title:"Date", field:"time"},
          {title:"Temperature", field:"dataset"},
          
    ]
});

var tableAirHumidity = new Tabulator("#airHumidityT", {
    reactiveData:true, //enable reactive data
    data:tableData, //assign data array
    layout:"fitDataTable",
    columns:[
          {title:"Date", field:"time"},
          {title:"Temperature", field:"dataset"},
    ]
});

var tableAirHumidity = new Tabulator("#airHumidityT", {
    reactiveData:true, //enable reactive data
    data:tableData, //assign data array
    layout:"fitDataTable",
    columns:[
          {title:"Date", field:"time"},
          {title:"Temperature", field:"dataset"},
    ]
});

var tableAirHumidityDaily = new Tabulator("#airHumidityDailyT", {
    reactiveData:true, //enable reactive data
    data:tableData, //assign data array
    layout:"fitDataTable",
    columns:[
          {title:"Date", field:"time"},
          {title:"Temperature", field:"dataset"},
    ]
});

var tableAirHumidityWeekly = new Tabulator("#airHumidityWeeklyT", {
    reactiveData:true, //enable reactive data
    data:tableData, //assign data array
    layout:"fitDataTable",
    columns:[
          {title:"Date", field:"time"},
          {title:"Temperature", field:"dataset"},
    ]
});

var tableAirHumidityMonthly = new Tabulator("#airHumidityMonthlyT", {
    reactiveData:true, //enable reactive data
    data:tableData, //assign data array
    layout:"fitDataTable",
    columns:[
          {title:"Date", field:"time"},
          {title:"Temperature", field:"dataset"},
    ]
});

var tableWaterTemp = new Tabulator("#waterTempT", {
    reactiveData:true, //enable reactive data
    data:tableData, //assign data array
    layout:"fitDataTable",
    columns:[
          {title:"Date", field:"time"},
          {title:"Temperature", field:"dataset"},
    ]
});

var tableWaterTempDaily = new Tabulator("#waterTempDailyT", {
    reactiveData:true, //enable reactive data
    data:tableData, //assign data array
    layout:"fitDataTable",
    columns:[
          {title:"Date", field:"time"},
          {title:"Temperature", field:"dataset"},
    ]
});

var tableWaterTempWeekly = new Tabulator("#waterTempWeeklyT", {
    reactiveData:true, //enable reactive data
    data:tableData, //assign data array
    layout:"fitDataTable",
    columns:[
          {title:"Date", field:"time"},
          {title:"Temperature", field:"dataset"},
    ]
});

var tableWaterTempMonthly = new Tabulator("#waterTempMonthlyT", {
    reactiveData:true, //enable reactive data
    data:tableData, //assign data array
    layout:"fitDataTable",
    columns:[
          {title:"Date", field:"time"},
          {title:"Temperature", field:"dataset"},
    ]
});

var tableWaterLvl = new Tabulator("#waterLvlT", {
    reactiveData:true, //enable reactive data
    data:tableData, //assign data array
    layout:"fitDataTable",
    columns:[
          {title:"Date", field:"time"},
          {title:"Temperature", field:"dataset"},
    ]
});

var tableWaterLvlDaily = new Tabulator("#waterLvlDailyT", {
    reactiveData:true, //enable reactive data
    data:tableData, //assign data array
    layout:"fitDataTable",
    columns:[
          {title:"Date", field:"time"},
          {title:"Temperature", field:"dataset"},
    ]
});

var tableWaterLvlWeekly = new Tabulator("#waterLvlWeeklyT", {
    reactiveData:true, //enable reactive data
    data:tableData, //assign data array
    layout:"fitDataTable",
    columns:[
          {title:"Date", field:"time"},
          {title:"Temperature", field:"dataset"},
    ]
});

var tableWaterLvlMonthly = new Tabulator("#waterLvlMonthlyT", {
    reactiveData:true, //enable reactive data
    data:tableData, //assign data array
    layout:"fitDataTable",
    columns:[
          {title:"Date", field:"time"},
          {title:"Temperature", field:"dataset"},
    ]
});

var tablepH = new Tabulator("#pHT", {
    reactiveData:true, //enable reactive data
    data:tableData, //assign data array
    layout:"fitDataTable",
    columns:[
          {title:"Date", field:"time"},
          {title:"Temperature", field:"dataset"},
    ]
});

var tablepHDaily = new Tabulator("#pHDailyT", {
    reactiveData:true, //enable reactive data
    data:tableData, //assign data array
    layout:"fitDataTable",
    columns:[
          {title:"Date", field:"time"},
          {title:"Temperature", field:"dataset"},
    ]
});

var tablepHWeekly = new Tabulator("#pHWeeklyT", {
    reactiveData:true, //enable reactive data
    data:tableData, //assign data array
    layout:"fitDataTable",
    columns:[
          {title:"Date", field:"time"},
          {title:"Temperature", field:"dataset"},
    ]
});

var tablepHMonthly = new Tabulator("#pHMonthlyT", {
    reactiveData:true, //enable reactive data
    data:tableData, //assign data array
    layout:"fitDataTable",
    columns:[
          {title:"Date", field:"time"},
          {title:"Temperature", field:"dataset"},
    ]
});

var tableNutrientLvl = new Tabulator("#nutrientLvlT", {
    reactiveData:true, //enable reactive data
    data:tableData, //assign data array
    layout:"fitDataTable",
    columns:[
          {title:"Date", field:"time"},
          {title:"Temperature", field:"dataset"},
    ]
});

var tableNutrientLvlDaily = new Tabulator("#nutrientLvlDailyT", {
    reactiveData:true, //enable reactive data
    data:tableData, //assign data array
    layout:"fitDataTable",
    columns:[
          {title:"Date", field:"time"},
          {title:"Temperature", field:"dataset"},
    ]
});

var tableNutrientLvlWeekly = new Tabulator("#nutrientLvlWeeklyT", {
    reactiveData:true, //enable reactive data
    data:tableData, //assign data array
    layout:"fitDataTable",
    columns:[
          {title:"Date", field:"time"},
          {title:"Temperature", field:"dataset"},
    ]
});

var tableNutrientLvlMonthly = new Tabulator("#nutrientLvlMonthlyT", {
    reactiveData:true, //enable reactive data
    data:tableData, //assign data array
    layout:"fitDataTable",
    columns:[
          {title:"Date", field:"time"},
          {title:"Temperature", field:"dataset"},
    ]
});



function addSummary(table,data,timestamps){
    let newData = [];
    let row = {};
    let lastTime = timestamps[0].dataset.length - 1;  // obtain index of last element
    row['time'] = timestamps[0].dataset[lastTime];
    row['dataset'] = data.dataset[lastTime];
    newData.push(row);
    table.setData(newData);
    table.redraw();
}

function addTableData(table, data,timestamps) {
    let newData = [];
    let row = {};
    for (let i = 0; i < data.dataset.length; i++) {
        row['time'] = timestamps[0].dataset[i];
        row['dataset'] = data.dataset[i];
        newData.push(row);
     }
    table.setData(newData);
    table.redraw
}

