var sheet = SpreadsheetApp.getActiveSheet();
var count = sheet.getRange(2,2).getValues();

function doGet(e) {
	// return HtmlService.createHtmlOutputFromFile('index');
    params = JSON.stringify(e);

    add(e.parameter["lat"], e.parameter["lng"])
    return HtmlService.createHtmlOutput(params);
}

function add(s1, s2) {
    sheet.deleteRow(sheet.getLastRow());
    Logger.log(count[0][0]);
    count[0][0] += 1; 
    sheet.appendRow([new Date(), count[0][0], s1, s2]);
}