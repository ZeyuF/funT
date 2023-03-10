/* Hector -- A Simple Climate Model
   Copyright (C) 2022  Battelle Memorial Institute

   Please see the accompanying file LICENSE.md for additional licensing
   information.
*/
#ifndef SULFUR_COMPONENT_H
#define SULFUR_COMPONENT_H
/*
 *  so2_component.hpp
 *  hector
 *
 *  Created by Corinne on 5/6/2013
 *
 */

#include "imodel_component.hpp"
#include "logger.hpp"
#include "tseries.hpp"
#include "unitval.hpp"

namespace Hector {
//------------------------------------------------------------------------------
/*! \brief Sulfur model component.
 *
 *  This doesn't do much yet.
 */
class SulfurComponent : public IModelComponent {

public:
  SulfurComponent();
  ~SulfurComponent();

  // IModelComponent methods
  virtual std::string getComponentName() const;

  virtual void init(Core *core);

  virtual unitval sendMessage(const std::string &message,
                              const std::string &datum,
                              const message_data info = message_data());

  virtual void setData(const std::string &varName, const message_data &data);

  virtual void prepareToRun();

  virtual void run(const double runToDate);

  virtual void reset(double time);

  virtual void shutDown();

  // IVisitable methods
  virtual void accept(AVisitor *visitor);

  unitval SN;

private:
  virtual unitval getData(const std::string &varName, const double date);

  //! Emissions time series

  tseries<unitval> SO2_emissions;
  tseries<unitval> SV;

  //! logger
  Logger logger;

  Core *core;
  double oldDate;
};

} // namespace Hector

#endif // SULFUR_COMPONENT_H
