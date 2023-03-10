/* Hector -- A Simple Climate Model
   Copyright (C) 2022  Battelle Memorial Institute

   Please see the accompanying file LICENSE.md for additional licensing
   information.
*/

#ifndef CH4_COMPONENT_H
#define CH4_COMPONENT_H
/*
 *  ch4_component.hpp
 *  hector
 *
 *  Created by Ben on 05/19/2011.
 *
 */

#include "imodel_component.hpp"
#include "logger.hpp"
#include "tseries.hpp"
#include "unitval.hpp"

namespace Hector {

//------------------------------------------------------------------------------
/*! \brief Methane model component.
 */
class CH4Component : public IModelComponent {

public:
  CH4Component();
  ~CH4Component();

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
  //! emissions time series
  tseries<unitval> CH4_emissions;
  tseries<unitval> CH4;           // CH4 concentrations, ppbv CH4
  tseries<unitval> CH4_constrain; // CH4 concentration constraint, ppbv CH4
  unitval M0;                     // preindustrial methane, ppbv CH4
  unitval UC_CH4; // conversion factor between emissions and concentration
  unitval CH4N;   // annual natural emissions, Tg CH4/yr
  unitval Tsoil;  // lifetime of soil sink, yr
  unitval Tstrat; // lifetime of tropospheric sink, yr

  // logger
  Logger logger;

  Core *core;
  double oldDate;
};

} // namespace Hector

#endif // CH4_COMPONENT_H
