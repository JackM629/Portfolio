package listeners;

import javax.servlet.ServletContext;
import javax.servlet.ServletContextEvent;
import javax.servlet.ServletContextListener;
import model.JDBCBean;

/**
 * Web application lifecycle listener.
 *
 * Created on : 27-Nov-2017, 13:16:36, Author: Frazer, Sasha, Jack
 */
public class JDBCListener implements ServletContextListener {

    //JDBC connection bean
    JDBCBean bean;

    @Override
    public void contextInitialized(ServletContextEvent event) {

        ServletContext context = event.getServletContext();
        JDBCBean bean = new JDBCBean();

        //Get JDBC Parameters
        String driver = context.getInitParameter("JDBC-Driver");
        String url = context.getInitParameter("JDBC-URL");
        String userName = context.getInitParameter("JDBC-UserName");
        String password = context.getInitParameter("JDBC-Password");

        //Start JDBC
        bean.startJDBC(driver, url, userName, password);

        //Pass JDBC connected bean to context
        context.setAttribute("JDBCBean", bean);
    }

    @Override
    public void contextDestroyed(ServletContextEvent sce) {
    }
}
