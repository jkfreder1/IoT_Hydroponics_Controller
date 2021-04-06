

//define array of table data
var tableData = [
    {date: 1}
]
var test = [
    {date: 2, dataset: 2}
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

var airTempTable = new Tabulator("#airTempT", {
    reactiveData:true, //enable reactive data
    data:tableData, //assign data array
    layout:"fitDataTable",

    columns:[
          {title:"Date", field:"time"},
          {title:"Temperature", field:"dataset"},
          
    ]
});
var airTempDailyTable = new Tabulator("#airTempDailyT", {
    reactiveData:true, //enable reactive data
    data:tableData, //assign data array
    layout:"fitDataTable",

    columns:[
          {title:"Date", field:"time"},
          {title:"Temperature", field:"dataset"},
          
    ]
});

var airTempWeeklyTable = new Tabulator("#airTempWeeklyT", {
    reactiveData:true, //enable reactive data
    data:tableData, //assign data array
    layout:"fitDataTable",

    columns:[
          {title:"Date", field:"time"},
          {title:"Temperature", field:"dataset"},
          
    ]
});

var airTempMonthlyTable = new Tabulator("#airTempMonthlyT", {
    reactiveData:true, //enable reactive data
    data:tableData, //assign data array
    layout:"fitDataTable",

    columns:[
          {title:"Date", field:"time"},
          {title:"Temperature", field:"dataset"},
          
    ]
});




function addTableData(table, data,timestamps) {
    var newData = [];
    var row = {};
    for (var i = 0; i < data.dataset.length; i++) {
        row['time'] = timestamps[i];
        row['dataset'] = data.dataset[i];
        newData.push(row);
     }
    table.setData(newData);
}

