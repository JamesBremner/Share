#include "cShare.h"

class cStarterGUI
{
public:
    /** CTOR
     * @param[in] title will appear in application window title
     * @param[in] vlocation set location and size of appplication window
     *
     * Usage:
     *
     * <pre>
class appGUI : public cStarterGUI
{
public:
    appGUI()
        : cStarterGUI(
              "The Appliccation",
              {50, 50, 1000, 500})
    {

        // initialize the solution
        ...

        show();
        run();
    }
    </pre>
    */
    cStarterGUI(
        const std::string &title,
        const std::vector<int> &vlocation)
        : fm(wex::maker::make())
    {
        fm.move(vlocation);
        fm.text(title);

        fm.events().draw(
            [&](PAINTSTRUCT &ps)
            {
                wex::shapes S(ps);
                draw(S);
            });
    }
    /** Draw nothing
     *
     * An application should over-ride this method
     * to perform any drawing reuired
     */
    virtual void draw(wex::shapes &S)
    {
    }
    void show()
    {
        fm.show();
    }
    void run()
    {
        fm.run();
    }

protected:
    wex::gui &fm;
};
class cGUI : public cStarterGUI
{
public:
    cGUI()
        : cStarterGUI(
              "Starter",
              {50, 50, 1000, 500})
    {
        menus();

        show();
        run();
    }

private:
    cShare S;

    void menus()
    {
        wex::menubar mb(fm);

        wex::menu mf(fm);
        mf.append("Open file",
                  [&](const std::string &title)
                  {
                      // prompt for file to open
                      wex::filebox fb(fm);
                      auto paths = fb.open();
                      if (paths.empty())
                          return;
                      readFile(S, paths);
                      solve(S);
                      std::cout << text(S);
                  });
        mf.append("Unit tests",
                  [&](const std::string &title)
                  {
                      if( ! test() ) {
                        wex::msgbox("Unit Tests Failed");
                        exit(1);
                      }
                      wex::msgbox("Unit Tests Passed");
                  });

        mb.append("File", mf);
    }
};