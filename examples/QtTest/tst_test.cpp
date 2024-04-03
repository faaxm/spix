#include <QtTest>

#include "anyrpc/anyrpc.h"

#include <memory>

using namespace anyrpc;

/**
 * @brief Example using QtTest for running test cases against a remote server
 *        It also contains some handy helper functions to call remote methods.
 *
 *        Run e.g. the RemoteCtrl application as the application under test
 */
class QtTestExample : public QObject {
    Q_OBJECT

    std::shared_ptr<XmlHttpClient> client;

public:
    QtTestExample();
    ~QtTestExample();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1();

    void waitFor(int time)
    {
        Value paramsWait;
        Value resultWait;
        paramsWait[0] = time;
        client->Call("wait", paramsWait, resultWait);
    }

    void mouseClick(std::string path)
    {
        Value paramsMouse;
        Value resultMouse;
        paramsMouse[0] = path;
        client->Call("mouseClick", paramsMouse, resultMouse);
        waitFor(500);
    }

    void screenshot(std::string path, std::string filepath, bool wait = true, bool compare = true)
    {
        Value params;
        Value result;
        params[0] = path;
        params[1] = filepath;
        client->Call("takeScreenshot", params, result);
    }

    void setProperty(std::string path, std::string field, std::string value)
    {
        Value params;
        Value result;
        params[0] = path;
        params[1] = field;
        params[2] = value;
        client->Call("setStringProperty", params, result);
    }

    QString getProperty(std::string path, std::string field)
    {
        Value params;
        Value result;
        params[0] = path;
        params[1] = field;
        client->Call("getStringProperty", params, result);

        return QString::fromStdString(result.GetString());
    }

    void invokeMethod(std::string path, std::string field, const std::vector<std::string> functionParameter)
    {
        Value params;
        Value result;

        Value parameters;
        parameters.SetArray(functionParameter.size());
        for (int i = 0; i < functionParameter.size(); ++i) {
            parameters[i] = functionParameter.at(i);
        }

        params[0] = path;
        params[1] = field;
        params[2] = parameters;
        client->Call("invokeMethod", params, result);
    }
};

QtTestExample::QtTestExample()
{
}

QtTestExample::~QtTestExample()
{
}

void QtTestExample::initTestCase()
{
    client = std::make_shared<anyrpc::XmlHttpClient>();
    client->SetServer("127.0.0.1", 9000);
    client->SetTimeout(10000);
}

void QtTestExample::cleanupTestCase()
{
}

void QtTestExample::test_case1()
{
    setProperty("mainWindow/results", "text", "");

    mouseClick("mainWindow/Button_1");
    waitFor(500);
    mouseClick("mainWindow/Button_2");
    waitFor(500);

    // in the "old" variant, this call also triggers mouseArea_2
    // this is somehow correct, because the mouse area is the top component,
    // but sometimes we do not want to trigger the top most component in test cases
    mouseClick("mainWindow/Button_3");
    waitFor(500);
    mouseClick("mainWindow/Button_4");
    waitFor(500);

    mouseClick("mainWindow/mouseArea_1");
    waitFor(500);
    mouseClick("mainWindow/mouseArea_2");
    waitFor(500);

    auto resultText = getProperty("mainWindow/results", "text");
    std::cout << resultText.toStdString() << std::endl;

    QCOMPARE(resultText,
        "Button 1 clicked\nButton 2 clicked\nButton 3 clicked\nButton 4 clicked\nMouseArea 1 clicked\nMouseArea 2 "
        "clicked");
}

QTEST_APPLESS_MAIN(QtTestExample)

#include "tst_test.moc"
