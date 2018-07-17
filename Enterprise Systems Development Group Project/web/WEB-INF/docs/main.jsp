<%-- 
    Document   : main
    Created on : 27-Nov-2017, 13:16:36
    Author     : Frazer, Sasha
--%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <link type="text/css" rel="stylesheet" href="${pageContext.request.contextPath}/css/main.css">
        <title>XYZ Online</title>
    </head>
    <body>
        <%@ include file="/resources/Header.jsp" %>
        <jsp:include page="${requestScope.included}" flush="true" />        
    </body>
</html>
