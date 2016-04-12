/**
 * Retrieves all the rows in the active spreadsheet that contain data and logs the
 * values for each row.
 * For more information on using the Spreadsheet API, see
 * https://developers.google.com/apps-script/service_spreadsheet
 */

function exportTexts() 
{
  var sheet = SpreadsheetApp.getActiveSpreadsheet().getSheetByName("Texts");
  var range = sheet.getDataRange();
  var data  = range.getValues();
  
  var exportMarkerColumnId = -1;
  var stringIDColumnId = -1;
  
  var langCount = 0;
  
  var headerRow = data[0];
  
  for(var i=0; i < headerRow.length; i++)
  {
    if(headerRow[i] == "Export")
    {
      exportMarkerColumnId = i;
    }
    else
    if(headerRow[i] == "ID")
    {
      stringIDColumnId = i;
    }
  }
  
  var charcount = 0;
  var charset = [];
  var out = "TextsDataBase <- \n{\n";
  
  for(var i = stringIDColumnId + 1; i < headerRow.length; i++)
  {
    if(headerRow[i] != "")
    {
      out += "\t" + headerRow[i] + " = \n\t{\n";
      
      for(var j=0; j<data.length; j++)
      {
        if(data[j][exportMarkerColumnId] == "+")
        {
          var str  = data[j][i];
          
          out += "\t\t" + data[j][stringIDColumnId] + " = " + "\"" + str + "\",\n"
          /*for(var k=0; k<str.length; k++)
          {
            charset[charcount++] = str.charAt(k);
          }*/
        }
      }
      
      out += "\t},\n";
      
    }
  }
  
  out += "};";
  
  if(charset.length > 0)
  {
    out += "\nCharset <- ";
    for(var i=0;i<charset.length;i++)
    {
      out += "\"" + charset[i] + "\", ";
    }
  }
  
  DocsList.createFile("textsdb.nut", out);
};

function exportObjects() 
{
  var sheet = SpreadsheetApp.getActiveSpreadsheet().getSheetByName("Objects");
  var range = sheet.getDataRange();
  var data  = range.getValues();
  
  var exportMarkerColumnId = -1;
  var classParamColumnId = -1;
  var paramTypeColumnId = -1;
  var objectFirstColumnId = -1;
  
  var headerRow = data[0];
  
  for(var i=0; i < headerRow.length; i++)
  {
    if(headerRow[i] == "Export")
    {
      exportMarkerColumnId = i;
    }
    if(headerRow[i] == "Class")
    {
      classParamColumnId = i;
    }
    if(headerRow[i] == "Type")
    {
      paramTypeColumnId = i;
    }
    if(headerRow[i] == "Objects")
    {
      objectFirstColumnId = i;
    }
  }
  
  var out = "ObjectsDataBase <- \n{\n";
  
  for(var i=0; i < data.length; i++)
  {
    if(data[i][exportMarkerColumnId] == "+")
    {
      //Export table
      
      var className = data[i][classParamColumnId];
      var paramCount = 0;
      var objectCount = 0;

      for(var j = i + 1; j < range.getNumRows(); j++)
      {
        if(data[j][classParamColumnId].toString() == "")
          break;
        
        paramCount++;
      }
      
      out += "\t" + className + " = \n\t{\n";
      
      for(var j = objectFirstColumnId; j < range.getNumColumns(); j++)
      {
        if(data[i][j].toString() == "")
          break;
        
        out += "\t\t" + data[i][j] + " = \n\t\t{\n";
        
        for(var k = 0; k < paramCount; k++)
        {
          var type = data[i+k+1][paramTypeColumnId];
          var value = data[i+k+1][j];
          
          if(type != "number")
          {
            value = "\"" + value + "\"";
          }
          
          out += "\t\t\t" + data[i+k+1][classParamColumnId] + " = " + value + ",\n";
        }
        
        out += "\t\t},\n";
        
        objectCount++;
      }
      
      out += "\t},\n";
    }
  }
  
  out += "};"
  
  DocsList.createFile("objectsdb.nut", out);
};

function onOpen() 
{
  var spreadsheet = SpreadsheetApp.getActiveSpreadsheet();
  var entries = [{
    name : "Export Objects",
    functionName : "exportObjects"
  },
  {
    name : "Export Texts",
    functionName : "exportTexts"
  }];
  spreadsheet.addMenu("WaloEngine", entries);
};
