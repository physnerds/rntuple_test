#ifndef DUNESOADATAMODEL_H
#define DUNESOADATAMODEL_H

#include "DataModel.h"
#include "SoALayout.h"
#include "SoAView.h"

#include <vector>

class DuneSoADataModel: DataModel<uint32_t>{
  public:
  DuneSoADataModel();
  ~DuneSoADataModel();
  
  //Start with a simple type for now...
  GENERATE_SOA_LAYOUT(SoATemplate,
                      
                      SOA_COLUMN(uint32_t, ch1),

                      SOA_SCALAR(const char*, description)
                      );
  
  
 GENERATE_SOA_LAYOUT(SoASingleTemplate,
                     SOA_COLUMN(uint32_t, ch1),
                     SOA_SCALAR(const char*, description)
                     );
 
  //std::vector<uint32_t>ReturnFakeData(std::size_t length) override;
  
  private:
  
};

#endif