#include "XMLUtils.hh"
#include "tinyxml.h"
#include "Debug.hh"
#include "Utils.hh"

/**
 * @author Conor McGann
 */

#include <fstream>
#include <sstream>

namespace EUROPA {

  std::string extractData(const TiXmlElement& configData, const std::string& argName){
    const char * data = configData.Attribute(argName);
    checkError(data != NULL, "No attribute '" << argName << "' in " << configData);
    return data;
  }

TiXmlElement* initXml(const char* sourceFile, const char* element) {
  TiXmlDocument* doc = new TiXmlDocument(sourceFile);
  doc->LoadFile();
  checkError(!doc->Error(), "Invalid or malformed XML file '" << sourceFile << "'");
  TiXmlElement * xmlElement = doc->RootElement();
  debugMsg("Tests", "Loading element " << *xmlElement);
  if(element == NULL)
    return xmlElement;
  while(xmlElement != NULL && strcmp(xmlElement->Value(), element) != 0)
    xmlElement = static_cast<TiXmlElement*>(xmlElement->NextSibling());
  if(xmlElement == NULL) {
    // here's a bit of a hack to do string concatination.
    // it's in an if statement so we don't construct the string otherwise.
    std::string message = "\"";
    message = message + element + "\" could not be found in " + sourceFile;
    assertTrue(xmlElement != NULL, message);
  }
  return xmlElement;
}

  TiXmlElement* initXml(std::string& xmlStr) {
    std::stringstream is;
    is << xmlStr;

    TiXmlElement* root = new TiXmlElement("");
    is >> (*root);
    return root;
  }

  const char* getTextChild (const TiXmlElement& element) {
    check_error(element.FirstChild(), "FirstChild is empty.");
    //    check_error(element.FirstChild()->ToComment(), "FirstChild->ToComment is empty");
    check_error(element.FirstChild()->ToText(), "FirstChild->ToText is empty.");
    check_error(element.FirstChild()->ToText()->Value(), "FirstChild->ToText->Value is empty.");

    return element.FirstChild()->ToText()->Value();
  }

  bool isNumber(const char* data){
    double dummy;
    return isNumber(data, dummy);
  }

  bool isNumber(const char* data, double& value){
    return toValue<double>(data, value);
  }
}
