ActionTypeArray::ActionTypeArray() {
  std::vector <ActionType> actionTypes;
}

unsigned int ActionTypeArray::add(ActionType *a) {
  actionTypes.push_back(*a);
  cout << currentTime() << " ActionType added: '" << a->name << "' (" << actionTypes.size() << " total action types)" << endl;
  return 0;
}

ActionType *ActionTypeArray::byName(string s) {
  for(std::vector<ActionType>::iterator it = actionTypes.begin(); it != actionTypes.end(); ++it) {
    if (it->name == s) {
      return &*it;
    }
  }
  return NULL;
}