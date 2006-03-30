#include "UNV2417_Structure.hxx"
#include "UNV_Utilities.hxx"

using namespace std;
using namespace UNV;
using namespace UNV2417;

#ifdef _DEBUG_
static int MYDEBUG = 1;
#else
static int MYDEBUG = 0;
#endif


static string _group_labels[] = {"2417", "2429", "2430", "2432", "2435"};
#define NBGROUP 5

//static string _label_dataset = "2435";

void UNV2417::Read(std::ifstream& in_stream, TDataSet& theDataSet)
{
  if(!in_stream.good())
    EXCEPTION(runtime_error,"ERROR: Input file not good.");

  std::string olds, news;
  
  while(true){
    in_stream >> olds >> news;
    /*
     * a "-1" followed by a number means the beginning of a dataset
     * stop combing at the end of the file
     */
    while( ((olds != "-1") || (news == "-1") ) && !in_stream.eof() ){	  
      olds = news;
      in_stream >> news;
    }
    if(in_stream.eof())
      return;
    for (int i = 0; i < NBGROUP; i++) {
      if (news == _group_labels[i]) {
	ReadGroup(news, in_stream, theDataSet);
      }
    }
  }
}



void UNV2417::ReadGroup(const std::string& myGroupLabel, std::ifstream& in_stream, TDataSet& theDataSet)
{
  TGroupId aId;
  for(; !in_stream.eof();){
    in_stream >> aId ;
    if(aId == -1){
      // end of dataset is reached
      break;
    }

    int n_nodes;
    TRecord aRec;
    int aTmp;
    in_stream>>aTmp; // miss not necessary values
    in_stream>>aTmp;
    in_stream>>aTmp;
    in_stream>>aTmp;
    in_stream>>aTmp;
    in_stream>>aTmp;
    in_stream>>n_nodes;

    in_stream>>aRec.GroupName;
    
    int aElType;
    int aElId;
    int aNum;
        for(int j=0; j < n_nodes; j++){
      in_stream>>aElType;
      in_stream>>aElId;
      if (myGroupLabel.compare("2435") == 0) {
	in_stream>>aTmp;
	in_stream>>aTmp;
      }
      switch (aElType) {
      case 7: // Nodes
	aNum = aRec.NodeList.size();
	aRec.NodeList.resize(aNum + 1);
	aRec.NodeList[aNum] = aElId;
	break;
      case 8: // Elements
	aNum = aRec.ElementList.size();
	aRec.ElementList.resize(aNum + 1);
	aRec.ElementList[aNum] = aElId;
	break;
      }
    }
    theDataSet.insert(TDataSet::value_type(aId,aRec));
  }

}
