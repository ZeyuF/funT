% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/messages.R
\name{setvar}
\alias{setvar}
\title{Set values for a Hector variable}
\usage{
setvar(core, dates, var, values, unit)
}
\arguments{
\item{core}{Hector core object}

\item{dates}{Vector of dates}

\item{var}{Capability string for the variable to set}

\item{values}{Values to set.  Must be either a single value or a vector the
same length as dates.}

\item{unit}{Unit string.  Can be set to NA for unitless variables.}
}
\description{
This function selects a variable by its capability name and sets the
requested values at the requested dates.  The units must also be supplied as
a single string (heterogeneous units are not supported).  These are checked
against the expected unit in the code, and an error is signaled if they don't
match (i.e., there is no attempt to convert units).
}
\seealso{
Other main user interface functions: 
\code{\link{fetchvars}()},
\code{\link{get_tracking_data}()},
\code{\link{newcore}()},
\code{\link{reset}()},
\code{\link{run}()},
\code{\link{shutdown}()}
}
\concept{main user interface functions}
