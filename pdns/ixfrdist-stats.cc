/*
 * This file is part of PowerDNS or dnsdist.
 * Copyright -- PowerDNS.COM B.V. and its contributors
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * In addition, for the avoidance of any doubt, permission is granted to
 * link this program with OpenSSL and to (re)distribute the binaries
 * produced as the result of such linking.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "ixfrdist-stats.hh"

std::string ixfrdistStats::getStats() {
  std::stringstream stats;
  const std::string prefix = "ixfrdist_";

  stats<<"# HELP "<<prefix<<"uptime_seconds The uptime of the process"<<std::endl;
  stats<<"# TYPE "<<prefix<<"uptime_seconds gauge"<<std::endl;
  stats<<prefix<<"uptime_seconds "<<time(nullptr) - progStats.startTime<<std::endl;

  stats<<"# HELP "<<prefix<<"domains The amount of configured domains"<<std::endl;
  stats<<"# TYPE "<<prefix<<"domains gauge"<<std::endl;
  stats<<prefix<<"domains "<<domainStats.size()<<std::endl;

  uint64_t numSOAChecks{0}, numSOAChecksFailed{0}, numSOAinQueries{0}, numIXFRinQueries{0}, numAXFRinQueries{0}, numAXFRFailures{0}, numIXFRFailures{0};
  bool helpAdded{false};
  for (auto const &d : domainStats) {
    if (!helpAdded) {
      stats<<"# HELP "<<prefix<<"soa_serial The SOA serial number of a domain"<<std::endl;
      stats<<"# TYPE "<<prefix<<"soa_serial gauge"<<std::endl;
    }
    if(d.second.haveZone)
      stats<<prefix<<"soa_serial{domain=\""<<d.first<<"\"} "<<d.second.currentSOA<<std::endl;
    else
      stats<<prefix<<"soa_serial{domain=\""<<d.first<<"\"} NaN"<<std::endl;

    if (!helpAdded) {
      stats<<"# HELP "<<prefix<<"soa_checks_total Number of times a SOA check at the master was attempted"<<std::endl;
      stats<<"# TYPE "<<prefix<<"soa_checks_total counter"<<std::endl;
    }
    stats<<prefix<<"soa_checks_total{domain=\""<<d.first<<"\"} "<<d.second.numSOAChecks<<std::endl;
    numSOAChecks += d.second.numSOAChecks;

    if (!helpAdded) {
      stats<<"# HELP "<<prefix<<"soa_checks_failed_total Number of times a SOA check at the master failed"<<std::endl;
      stats<<"# TYPE "<<prefix<<"soa_checks_failed_total counter"<<std::endl;
    }
    stats<<prefix<<"soa_checks_failed_total{domain=\""<<d.first<<"\"} "<<d.second.numSOAChecksFailed<<std::endl;
    numSOAChecksFailed += d.second.numSOAChecksFailed;

    if (!helpAdded) {
      stats<<"# HELP "<<prefix<<"soa_inqueries_total Number of times a SOA query was received"<<std::endl;
      stats<<"# TYPE "<<prefix<<"soa_inqueries_total counter"<<std::endl;
    }
    stats<<prefix<<"soa_inqueries_total{domain=\""<<d.first<<"\"} "<<d.second.numSOAinQueries<<std::endl;
    numSOAinQueries += d.second.numSOAinQueries;

    if (!helpAdded) {
      stats<<"# HELP "<<prefix<<"axfr_inqueries_total Number of times an AXFR query was received"<<std::endl;
      stats<<"# TYPE "<<prefix<<"axfr_inqueries_total counter"<<std::endl;
    }
    stats<<prefix<<"axfr_inqueries_total{domain=\""<<d.first<<"\"} "<<d.second.numAXFRinQueries<<std::endl;
    numAXFRinQueries += d.second.numAXFRinQueries;

    if (!helpAdded) {
      stats<<"# HELP "<<prefix<<"axfr_failures_total Number of times an AXFR query was not properly answered"<<std::endl;
      stats<<"# TYPE "<<prefix<<"axfr_failures_total counter"<<std::endl;
    }
    stats<<prefix<<"axfr_failures_total{domain=\""<<d.first<<"\"} "<<d.second.numAXFRFailures<<std::endl;
    numAXFRFailures += d.second.numAXFRFailures;

    if (!helpAdded) {
      stats<<"# HELP "<<prefix<<"ixfr_inqueries_total Number of times an IXFR query was received"<<std::endl;
      stats<<"# TYPE "<<prefix<<"ixfr_inqueries_total counter"<<std::endl;
    }
    stats<<prefix<<"ixfr_inqueries_total{domain=\""<<d.first<<"\"} "<<d.second.numIXFRinQueries<<std::endl;
    numIXFRinQueries += d.second.numIXFRinQueries;

    if (!helpAdded) {
      stats<<"# HELP "<<prefix<<"ixfr_failures_total Number of times an IXFR query was not properly answered"<<std::endl;
      stats<<"# TYPE "<<prefix<<"ixfr_failures_total counter"<<std::endl;
    }
    stats<<prefix<<"ixfr_failures_total{domain=\""<<d.first<<"\"} "<<d.second.numIXFRFailures<<std::endl;
    numIXFRFailures += d.second.numIXFRFailures;
    helpAdded = true;
  }

  return stats.str();
}
