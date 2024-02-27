#include "csv_parser/csvdocument.h"
#include <gtest/gtest.h>

namespace
{
struct TestParam
{
    bool error;
    std::string csvString;
    QList<QStringList> csvData;
};

std::vector<TestParam> getTestParams()
{
    std::vector<TestParam> testParams;

    testParams.push_back(
        { false,
          R"(

"Status",   "Icon",           "Extension"
"FALSE",    "icon_name.png",  "pdf,csv"

)",
          { { "Status", "Icon", "Extension" }, { "FALSE", "icon_name.png", "pdf,csv" } } });

    testParams.push_back({ true,
                           R"(
"Status",   "Icon",             "Extension"
"FALSE",    "icon_name.png",    "pdf
csv"

)",
                           {} }); // newlines not allowed inside quoted values (can be changed)

    return testParams;
}

class CsvLibTestLoad : public testing::TestWithParam<TestParam>
{
};

TEST_P(CsvLibTestLoad, fromString)
{
    bool error = false;
    auto [csvError, csvString, csvData] = GetParam();
    QtCSV::CsvDocument csv = QtCSV::fromString(csvString.c_str(), &error);
    ASSERT_EQ(error, csvError);
    if (!error)
        ASSERT_EQ(csv.getData(), csvData);
}

INSTANTIATE_TEST_SUITE_P(CsvLibTestLoadInstantiation, CsvLibTestLoad,
                         ::testing::ValuesIn(getTestParams()));

}; // namespace
