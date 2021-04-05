var liveColumns = [
    {title:"Date", field:"date"},
    {title:"Temperature", field: "dataset"}
]
var tableData = [
    {id:1, name:"Billy Bob", age:"12", gender:"male", height:1, col:"red", dob:"", cheese:1},
    {id:2, name:"Mary May", age:"1", gender:"female", height:2, col:"blue", dob:"14/05/1982", cheese:true},
]


var tableAirTemp = new Tabulator("#example-table", {
    data:tableData,
    columns:[
        {title:"Date", field:"date"},
        {title:"Temperature", field: "dataset"}
    ],
});


function addTableData(table, data) {
    table.setData(data);
}