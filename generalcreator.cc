#include "generalcreator.h"
using namespace std;

GeneralCreator::GeneralCreator(shared_ptr<Floor> floorMap): Creator{floorMap} {}

GeneralCreator::~GeneralCreator() {};
