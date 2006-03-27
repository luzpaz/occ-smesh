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

static string _label_dataset = "2417";

void UNV2417::Read(std::ifstream& in_stream, TDataSet& theDataSet)
{
  if(!in_stream.good())
    EXCEPTION(runtime_error,"ERROR: Input file not good.");

  /*
   * adjust the \p istream to our
   * position
   */
  if(!beginning_of_dataset(in_stream,_label_dataset))
    EXCEPTION(runtime_error,"WARNING: Could not find "<<_label_dataset<<" dataset!");

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
    in_stream>>n_nodes;

    int aElType;
    int aElId;
    int aNum;
    for(int j=0; j < n_nodes; j++){
      in_stream>>aElType;
      in_stream>>aElId;
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
