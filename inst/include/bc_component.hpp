/* Hector -- A Simple Climate Model
   Copyright (C) 2022  Battelle Memorial Institute

   Please see the accompanying file LICENSE.md for additional licensing
   information.
*/
#ifndef BLACK_CARBON_COMPONENT_H
#define BLACK_CARBON_COMPONENT_H
/*
 *  bc_component.hpp
 *  hector
 *
 *  Created by Ben on 05/26/2011.
 *
 */

#include "imodel_component.hpp"
#include "logger.hpp"
#include "tseries.hpp"
#include "unitval.hpp"

namespace Hector {

//------------------------------------------------------------------------------
/*! \brief Black carbon model component.
 *
 *  This doesn't do much yet.
 */
class BlackCarbonComponent : public IModelComponent {

public:
  BlackCarbonComponent();
  ~BlackCarbonComponent();

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

private:
  virtual unitval getData(const std::string &varName, const double date);

  //! Emissions time series
  tseries<unitval> BC_emissions;

  //! logger
  Logger logger;

  Core *core;
  double oldDate;
};

} // namespace Hector

#endif // BLACK_CARBON_COMPONENT_H
