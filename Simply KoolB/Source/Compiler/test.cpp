std::string Database::ListFunctions() {
    std::string Functions = "Exit";
    map<std::string, SubFunctionInfo>::const_iterator Item;
    Item = Scope[ScopeLevel].SubFunctions.begin();
    while(Item != Scope[ScopeLevel].SubFunctions.end()) {
        if (Item->second.External == false) {
            Functions += "," + Item->second.Name;
        }
        ++Item;
    }
    return Functions;
}