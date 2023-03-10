/* Hector -- A Simple Climate Model
   Copyright (C) 2022  Battelle Memorial Institute

   Please see the accompanying file LICENSE.md for additional licensing
   information.
*/
#ifndef OZONE_COMPONENT_H
#define OZONE_COMPONENT_H
/*
 *  ozone_component.hpp
 *  hector
 *
 *  Created by Ben on 10/24/2013.
 *
 */

#include "imodel_component.hpp"
#include "logger.hpp"
#include "tseries.hpp"
#include "unitval.hpp"

namespace Hector {
//------------------------------------------------------------------------------
/*! \brief Ozone model component.
 *
 *  This doesn't do much yet.
 */
class OzoneComponent : public IModelComponent {

public:
  OzoneComponent();
  ~OzoneComponent();

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

  //! Current ozone concentration, relative to preindustrial, Dobson units
  unitval PO3;
  tseries<unitval> O3;
  tseries<unitval> CO_emissions;
  tseries<unitval> NMVOC_emissions;
  tseries<unitval> NOX_emissions;

  //! logger
  Logger logger;

  Core *core;
  double oldDate;
};

} // namespace Hector

#endif // OZONE_COMPONENT_H
