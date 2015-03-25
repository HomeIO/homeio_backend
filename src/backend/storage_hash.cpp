StorageHash::StorageHash(unsigned long long tF, unsigned long long tT, double v) {
  timeFrom = tF;
  timeTo = tT;
  value = v;
  
  cout << "new SH " << value << " " << timeFrom << " " << timeTo << endl;
}