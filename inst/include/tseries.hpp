/* Hector -- A Simple Climate Model
   Copyright (C) 2022  Battelle Memorial Institute

   Please see the accompanying file LICENSE.md for additional licensing
   information.
*/
#ifndef TSERIES_H
#define TSERIES_H
/*
 *  tseries.h - the basic time series data type
 *  hector
 *
 *  Created by Ben on 9/14/10.
 *
 */

#include <limits>
#include <map>
#include <sstream>

#include "fluxpool.hpp"
#include "h_exception.hpp"
#include "h_interpolator.hpp"
#include "logger.hpp"
#include "unitval.hpp"

namespace Hector {

/*! \brief Time series data type.
 *
 *  Currently implemented as an STL map.
 */
template <class T_data> class tseries {
  std::map<double, T_data> mapdata;
  double lastInterpYear;
  bool endinterp_allowed;
  mutable bool dirty; // does series need re-interpolating?

  h_interpolator interpolator;
  void set_interp(double, bool, interpolation_methods);
  void fit_spline();

public:
  tseries();

  void set(double, T_data);
  T_data get(double) const;
  T_data get_deriv(double) const;
  bool exists(double) const;

  double firstdate() const;
  double lastdate() const;

  int size() const;

  void allowInterp(bool eia);
  void allowPartialInterp(bool eia);

  void truncate(double t, bool after = true);

  std::string name;
};

//-----------------------------------------------------------------------
/*! \brief A helper class to allow template specialization for interpolation.
 *
 * The interpolator expects types to be double convertible and will not compile
 * if the templated type can not be implicitly converted (or unless a
 * specialization has been created, as for strings below).  Creating a
 * specialization of this class to explicitly convert to double will allow the
 * use of tseries with that type and run as expected including interpolation.
 */
template <class T_data> struct interp_helper {
  // TODO: we might want to consider re-organizing this to not have to pass
  // info around, discuss with Ben
  static void error_check(const std::map<double, T_data> &userData,
                          h_interpolator &interpolator, std::string name,
                          bool &isDirty, bool endinterp_allowed,
                          const double index) {
    H_ASSERT(userData.size() > 1,
             "time series data(" + name + ") must have size>1");

    if (isDirty) { // data have changed; inform interpolator
      double *x = new double[userData.size()]; // allocate
      double *y = new double[userData.size()];

      typename std::map<double, T_data>::const_iterator itr; // ...and fill
      int i = 0;
      for (auto datum : userData) {
        x[i] = datum.first;
        y[i] = datum.second;
        i++;
      }

      interpolator.newdata(i, x, y);

      delete[] x; // spline will keep its own copy
      delete[] y; // spline will keep its own copy
      isDirty = false;
    }

    if (index < (*userData.begin()).first ||
        index > (*userData.rbegin()).first) // beyond-end interpolation
      H_ASSERT(endinterp_allowed,
               "In time series '" + name + "', end interpolation not allowed");
  }
  static T_data interp(const std::map<double, T_data> &userData,
                       h_interpolator &interpolator, std::string name,
                       bool &isDirty, bool endinterp_allowed,
                       const double index) {
    error_check(userData, interpolator, name, isDirty, endinterp_allowed,
                index);

    return interpolator.f(index);
  }
  static T_data calc_deriv(const std::map<double, T_data> &userData,
                           h_interpolator &interpolator, std::string name,
                           bool &isDirty, bool endinterp_allowed,
                           const double index) {
    error_check(userData, interpolator, name, isDirty, endinterp_allowed,
                index);

    return interpolator.f_deriv(index);
  }
};

//-----------------------------------------------------------------------
/*! \brief Interpolation specialization for unitvals.
 */
template <> struct interp_helper<unitval> {
  typedef unitval T_unit_type;
  // TODO: we might want to consider re-organizing this to not have to pass
  // info around, discuss with Ben
  static void error_check(const std::map<double, T_unit_type> &userData,
                          h_interpolator &interpolator, std::string name,
                          bool &isDirty, bool endinterp_allowed,
                          const double index) {
    H_ASSERT(userData.size() > 1,
             "time series data (" + name + ") must have size>1");

    if (isDirty) { // data have changed; inform interpolator
      double *x = new double[userData.size()]; // allocate
      double *y = new double[userData.size()];

      int i = 0;
      for (auto datum : userData) {
        x[i] = datum.first;
        y[i] = datum.second.value(datum.second.units());
        i++;
      }

      interpolator.newdata(i, x, y);

      delete[] x; // spline will keep its own copy
      delete[] y; // spline will keep its own copy
      isDirty = false;
    }

    if (index < (*userData.begin()).first ||
        index > (*userData.rbegin()).first) // beyond-end interpolation
      H_ASSERT(endinterp_allowed, "end interpolation not allowed");
  }
  static T_unit_type interp(const std::map<double, T_unit_type> &userData,
                            h_interpolator &interpolator, std::string name,
                            bool &isDirty, bool endinterp_allowed,
                            const double index) {
    error_check(userData, interpolator, name, isDirty, endinterp_allowed,
                index);

    return unitval(interpolator.f(index), (*(userData.begin())).second.units());
  }
  static T_unit_type calc_deriv(const std::map<double, T_unit_type> &userData,
                                h_interpolator &interpolator, std::string name,
                                bool &isDirty, bool endinterp_allowed,
                                const double index) {
    error_check(userData, interpolator, name, isDirty, endinterp_allowed,
                index);

    return unitval(interpolator.f_deriv(index),
                   (*(userData.begin())).second.units());
  }
};

//-----------------------------------------------------------------------
/*! \brief Interpolation specialization for fluxpools.
 */
template <> struct interp_helper<fluxpool> {
  typedef fluxpool T_unit_type;
  // TODO: we might want to consider re-organizing this to not have to pass
  // info around, discuss with Ben
  static void error_check(const std::map<double, T_unit_type> &userData,
                          h_interpolator &interpolator, std::string name,
                          bool &isDirty, bool endinterp_allowed,
                          const double index) {
    H_ASSERT(userData.size() > 1,
             "time series data (" + name + ") must have size>1");

    if (isDirty) { // data have changed; inform interpolator
      double *x = new double[userData.size()]; // allocate
      double *y = new double[userData.size()];

      int i = 0;
      for (auto datum : userData) {
        x[i] = datum.first;
        y[i] = datum.second.value(datum.second.units());
        i++;
      }

      interpolator.newdata(i, x, y);

      delete[] x; // spline will keep its own copy
      delete[] y; // spline will keep its own copy
      isDirty = false;
    }

    if (index < (*userData.begin()).first ||
        index > (*userData.rbegin()).first) // beyond-end interpolation
      H_ASSERT(endinterp_allowed, "end interpolation not allowed");
  }
  static T_unit_type interp(const std::map<double, T_unit_type> &userData,
                            h_interpolator &interpolator, std::string name,
                            bool &isDirty, bool endinterp_allowed,
                            const double index) {
    error_check(userData, interpolator, name, isDirty, endinterp_allowed,
                index);

    return fluxpool(interpolator.f(index),
                    (*(userData.begin())).second.units());
  }
  static T_unit_type calc_deriv(const std::map<double, T_unit_type> &userData,
                                h_interpolator &interpolator, std::string name,
                                bool &isDirty, bool endinterp_allowed,
                                const double index) {
    error_check(userData, interpolator, name, isDirty, endinterp_allowed,
                index);

    return fluxpool(interpolator.f_deriv(index),
                    (*(userData.begin())).second.units());
  }
};

//-----------------------------------------------------------------------
/*! \brief Interpolation specialization for strings.
 */
template <> struct interp_helper<string> {
  typedef string T_unit_type;
  static void error_check(const std::map<double, T_unit_type> &userData,
                          h_interpolator &interpolator, std::string name,
                          bool &isDirty, bool endinterp_allowed,
                          const double index) noexcept(false) {
    H_THROW("String interpolation not allowed");
  }
  static T_unit_type interp(const std::map<double, T_unit_type> &userData,
                            h_interpolator &interpolator, std::string name,
                            bool &isDirty, bool endinterp_allowed,
                            const double index) noexcept(false) {
    H_THROW("String interpolation not allowed");
  }
  static T_unit_type calc_deriv(const std::map<double, T_unit_type> &userData,
                                h_interpolator &interpolator, std::string name,
                                bool &isDirty, bool endinterp_allowed,
                                const double index) noexcept(false) {
    H_THROW("String derivative not allowed");
  }
};

/*  "Because templates are compiled when required, this forces a restriction
    for multi-file projects: the implementation (definition) of a template
    class or function must be in the same file as its declaration. That
    means that we cannot separate the interface in a separate header file,
    and that we must include both interface and implementation in any file
    that uses the templates."   http://www.cplusplus.com/doc/tutorial/templates/
*/

//-----------------------------------------------------------------------
/*! \brief Constructor for time series data type.
 *
 *  Initializes internal variables.
 */
template <class T_data> tseries<T_data>::tseries() {
  set_interp(std::numeric_limits<double>::min(), false,
             DEFAULT); // default values
  dirty = false;
  name = "?";
}

//-----------------------------------------------------------------------
/*! \brief 'Set' for time series data type.
 *
 *  Sets an (t, d) tuple, data d at time t.
 */
template <class T_data> void tseries<T_data>::set(double t, T_data d) {
  mapdata[t] = d;
  if (t < lastInterpYear) {
    dirty = true;
  }
}

//-----------------------------------------------------------------------
/*! \brief Does data exist at time (position) t?
 *
 *  Returns a bool to indicate if data exists.
 */
template <class T_data> bool tseries<T_data>::exists(double t) const {
  return (mapdata.find(t) != mapdata.end());
}

//-----------------------------------------------------------------------
/*! \brief 'Get' for time series data type.
 *
 *  Returns data associated with time t.
 *  If no value exists, behavior is defined by 'interp_allowed'.
 *
 *  If there is only a single value in the time series, then treat it
 *  as a constant (i.e, return the single value that we have).
 */
template <class T_data> T_data tseries<T_data>::get(double t) const {
  if (mapdata.size() == 1) {
    return mapdata.begin()->second;
  }
  typename std::map<double, T_data>::const_iterator itr = mapdata.find(t);
  if (itr != mapdata.end())
    return (*itr).second;
  else if (t < lastInterpYear)
    return interp_helper<T_data>::interp(
        mapdata, const_cast<tseries *>(this)->interpolator, name, dirty,
        endinterp_allowed, t);
  else {
    std::ostringstream errmsg;
    errmsg << "Interpolation requested but not allowed (" << name
           << ") date: " << t << "\n";
    H_THROW(errmsg.str());
  }
}

//-----------------------------------------------------------------------
/*! \brief Get the derivative of the series at time t.
 *
 *  Note interpolation must be allowed to calculate a derivative.
 *
 */
template <class T_data> T_data tseries<T_data>::get_deriv(double t) const {
  if (mapdata.size() == 1) {
    H_THROW("More than one data point needed to calculate a derivative");
  }

  if (t < lastInterpYear) {
    return interp_helper<T_data>::calc_deriv(
        mapdata, const_cast<tseries *>(this)->interpolator, name, dirty,
        endinterp_allowed, t);
  } else {
    std::ostringstream errmsg;
    errmsg << "Derivative requested but not allowed (" << name
           << ") date: " << t << "\n";
    H_THROW(errmsg.str());
  }
}

//-----------------------------------------------------------------------
/*! \brief Set interpolation policies for data.
 *
 *  Two parameters: Max date for which interpolation is allowed. And is end-
 *  interpolation allowed?
 */
template <class T_data>
void tseries<T_data>::set_interp(double ia, bool eia, interpolation_methods m) {
  lastInterpYear = ia;
  endinterp_allowed = eia;
  interpolator.set_method(m);
  dirty = true;
}

//-----------------------------------------------------------------------
/*! \brief Set interpolation policies to allow interpolation at any date.
 *
 * \param eia Whether to allow extrapolation.
 */
template <class T_data> void tseries<T_data>::allowInterp(bool eia) {
  set_interp(std::numeric_limits<double>::max(), eia, DEFAULT);
}

//-----------------------------------------------------------------------
/*! \brief Only allow interpolation for dates less that the current max date.
 *
 * \param eia Whether to allow extrapolation.  This would only apply for the
 *            low-end range.
 */
template <class T_data> void tseries<T_data>::allowPartialInterp(bool eia) {
  set_interp(lastdate(), eia, DEFAULT);
}

//-----------------------------------------------------------------------
/*! \brief Return index of first element in series.
 *
 *  Return index of first element in series.
 */
template <class T_data> double tseries<T_data>::firstdate() const {
  H_ASSERT(!mapdata.empty(), "no mapdata");
  return (*mapdata.begin()).first;
}

//-----------------------------------------------------------------------
/*! \brief Return index of last element in series.
 *
 *  Return index of last element in series.
 */
template <class T_data> double tseries<T_data>::lastdate() const {
  H_ASSERT(!mapdata.empty(), "no mapdata");
  return (*mapdata.rbegin()).first;
}

//-----------------------------------------------------------------------
/*! \brief Return size of series.
 *
 *  Return size of series.
 */
template <class T_data> int tseries<T_data>::size() const {
  return int(mapdata.size());
}

/*! \brief truncate a time series
 *
 *  \details The default is to wipe all of the data in the time series
 *           after the input date.  By setting the optional after
 *           argument to false, you can wipe data before the input
 *           date instead.
 *  \note If you're going to overwrite previously read-in data, and
 *        you're not supplying input at every year, then you need to
 *        trigger this function (probably by sending a M_TRUNCATE
 *        message to the component that owns the time series) in order
 *        to ensure that you don't have bad data in the in-between
 *        years.
 *  \note I haven't actually implemented the M_TRUNCATE message yet.
 *        We'd like to be able to read in an entire time series for
 *        all gasses and then discard the ones that GCAM will be
 *        providing emissions for.  Unfortunately, there's this
 *        horrible snafu where the GCAM climate model components have
 *        no way of knowing what emissions they will be getting until
 *        the model is running, and emissions start coming in.
 *        Eventually we may require GCAM to register the gasses it
 *        plans to provide emissions for at setup time.  For the time
 *        being we will just ensure that the hector ini file is
 *        compatible with whatever GCAM is doing.
 */
template <class T> void tseries<T>::truncate(double t, bool after) {
  typename std::map<double, T>::iterator it1, it2;
  if (after) {
    it1 = mapdata.upper_bound(t);
    it2 = mapdata.end();
  } else {
    it1 = mapdata.begin();
    it2 = mapdata.lower_bound(t);
  }
  mapdata.erase(it1, it2);
}

} // namespace Hector

#endif
