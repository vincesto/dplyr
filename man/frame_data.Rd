% Generated by roxygen2 (4.1.1): do not edit by hand
% Please edit documentation in R/frame-data.R
\name{frame_data}
\alias{frame_data}
\alias{tibble}
\title{Row-wise data_frame creation}
\usage{
frame_data(...)

tibble(...)
}
\arguments{
\item{...}{Arguments specifying the structure of a \code{data_frame}.}
}
\description{
Create a row-wise \code{\link{data_frame}}.
}
\examples{
frame_data(
  ~colA, ~colB,
  "a",   1,
  "b",   2
)
}

