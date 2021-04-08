

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


function addSummary(table,data,timestamps){
    let newData = [];
    let row = {};
    row['time'] = timestamps[23];
    row['dataset'] = data.dataset[23];
    newData.push(row);
    table.setData(newData);
}

function addTableData(table, data,timestamps) {
    let newData = [];
    let row = {};
    for (let i = 0; i < data.dataset.length; i++) {
        row['time'] = timestamps[i];
        row['dataset'] = data.dataset[i];
        newData.push(row);
     }
    table.setData(newData);
}

