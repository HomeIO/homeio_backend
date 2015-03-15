MeasTypeArray::MeasTypeArray() {
  std::vector <MeasType> measTypes;
}

unsigned int MeasTypeArray::add(MeasType *m) {
  measTypes.push_back(*m);
  cout << "MeasType added: '" << m->name << "' (" << measTypes.size() << " total meas types)" << endl;
  return 0;
}

MeasType *MeasTypeArray::byName(string s) {
  for(std::vector<MeasType>::iterator it = measTypes.begin(); it != measTypes.end(); ++it) {
    if (it->name == s) {
      return &*it;
    }
  }
  return NULL;
}