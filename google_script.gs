var ss = SpreadsheetApp.getActiveSpreadsheet();
var mainSheet = ss.getSheets()[0];
var historySheet = ss.getSheets()[1];
var count = mainSheet.getRange(2,2).getValues();

function doGet(e) {
	// return HtmlService.createHtmlOutputFromFile('index');
    params = JSON.stringify(e);

    add(e.parameter["lat"], e.parameter["lng"])
    return HtmlService.createHtmlOutput(params);
}

function add(s1, s2) {
	// update in mainSheet
    mainSheet.deleteRow(mainSheet.getLastRow());
    count[0][0] += 1; 
    mainSheet.appendRow([new Date(), count[0][0], s1, s2]);
    
    // append in historySheet
    historySheet.appendRow([new Date(), s1, s2]);
}