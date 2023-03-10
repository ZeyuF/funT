% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/hector.R
\docType{package}
\name{hector-package}
\alias{hector}
\alias{hector-package}
\title{hector: The Hector Simple Climate Model}
\description{
Provides an R interface for the Hector Simple Climate Model. Using this interface you can set up and initialize the model, change model parameters and emissions inputs, run Hector, and retrieve model outputs. Note that the package authors are not identical to the C++ model authors.
}
\details{
This package allows you to run the Hector Simple Climate Model (SCM) from R
and receive the results back as R data frames.
}
\section{Getting started}{

To run a scenario, you will need a scenario input file.  Several examples are
included in the \code{input} directory, which you can find with the
expression \code{system.file('input', package='hector')}.  The input files
have names like  \code{'hector_sspNNN.ini'}, where \code{NNN} is one of
\code{119}, \code{126}, \code{245}, \code{370}, \code{434}, \code{460}, or \code{ssp585}.

There is a quickstart wrapper called \code{\link{runscenario}}, which takes
an input file as an argument, runs the scenario, and returns the results in a
data frame.  Which variables you get out are controlled by an option setting.
Setting this option is explained in the manual page for
\code{\link{fetchvars}}.
}

\section{Advanced usage}{

You can get a lot more control over your hector run if you create a hector
instance (sometimes called a "core" elsewhere in this documentation) using
the \code{\link{newcore}} function.  Once you have that you can run Hector up
to specific dates, fetch results, set parameters or input data, rewind to an
earlier date so you can rerun (e.g., with changed emissions or parameters).
When you are finished with a hector instance, you should call
\code{\link{shutdown}} on it to release the resources it has allocated.
}

\references{
Hartin, C. A., Patel, P., Schwarber, A., Link, R. P., and Bond-Lamberty,
B. P.: A simple object-oriented and open-source model for scientific and
policy analyses of the global climate system - Hector v1.0, Geosci. Model
Dev., 8, 939-955, \url{https://doi.org/10.5194/gmd-8-939-2015}, 2015.

Hartin, C. A., Bond-Lamberty, B., Patel, P., and Mundra, A.: Ocean
acidification over the next three centuries using a simple global climate
carbon-cycle model: projections and sensitivities, Biogeosciences, 13,
4329-4342, \url{https://doi.org/10.5194/bg-13-4329-2016}, 2016.
}
\seealso{
\code{\link{runscenario}}, \code{\link{newcore}},
\code{\link{shutdown}}, \code{\link{fetchvars}}
}
\author{
\strong{Maintainer}: Kalyn Dorheim \email{kalyn.dorheim@pnnl.gov} (\href{https://orcid.org/0000-0001-8093-8397}{ORCID})

Authors:
\itemize{
  \item Ben Bond-Lamberty \email{bondlamberty@pnnl.gov} (\href{https://orcid.org/0000-0001-9525-4633}{ORCID})
}

Other contributors:
\itemize{
  \item Skylar Gering \email{sgering@g.hmc.edu} (\href{https://orcid.org/0000-0003-1974-3966}{ORCID}) [contributor]
  \item Corinne Hartin \email{hartin.corinne@epa.gov} (\href{https://orcid.org/0000-0003-1834-6539}{ORCID}) [contributor]
  \item Robert Link (\href{https://orcid.org/0000-0002-7071-248X}{ORCID}) [contributor]
  \item Mat Nicholson (mnichol3) [contributor]
  \item Pralit Patel \email{pralit.patel@pnnl.gov} (\href{https://orcid.org/0000-0003-3992-1061}{ORCID}) [contributor]
  \item Leeya Pressburger \email{leeya.pressburger@pnnl.gov} (\href{https://orcid.org/0000-0002-6850-2504}{ORCID}) [contributor, data contributor]
  \item Alexey Shiklomanov \email{alexey.shiklomanov@nasa.gov} (\href{https://orcid.org/0000-0003-4022-5979}{ORCID}) [contributor]
  \item Benjamin Vega-Westhoff (\href{https://orcid.org/0000-0001-7881-8388}{ORCID}) [contributor]
  \item Dawn Woodard \email{dawn.woodard@pnnl.gov} (\href{https://orcid.org/0000-0002-0468-4660}{ORCID}) [contributor]
}

}
\keyword{internal}
