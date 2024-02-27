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
          R"("Status","Icon","Extension"
"FALSE","icon_name.png","pdf,csv")",
          { { "Status", "Icon", "Extension" }, { "FALSE", "icon_name.png", "pdf,csv" } } });

    testParams.push_back(
        { true,
          R"("Status","Icon","Extension"
"FALSE","icon_name.png","pdf,csv"
)",
          { { "Status", "Icon", "Extension" },
            { "FALSE", "icon_name.png",
              "pdf,csv" } } }); // produced string does not have new line at the end of it

    return testParams;
}

class CsvLibTestSave : public testing::TestWithParam<TestParam>
{
};

TEST_P(CsvLibTestSave, toString)
{
    auto [csvError, csvString, csvData] = GetParam();
    QtCSV::CsvDocument csv = QtCSV::CsvDocument(csvData);
    QString string1 = QtCSV::toString(csv);
    QString string2(csvString.c_str());
    ASSERT_EQ(csvError, (string1 != string2));
}

INSTANTIATE_TEST_SUITE_P(CsvLibTestSaveInstantiation, CsvLibTestSave,
                         ::testing::ValuesIn(getTestParams()));

}; // namespace
