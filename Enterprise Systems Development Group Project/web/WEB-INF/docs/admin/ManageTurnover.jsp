<%-- 
    Document   : Turnover
    Created on : 27-Nov-2017, 13:16:36
    Author     : Frazer, Sasha, Jack
--%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ include file="/resources/AdminDashboardNavBar.jsp" %>

<div class="content">
    <h1>Manage XYZ Finances</h1>

    <div style="display: none;">
        ${JDBCBean.executeSQLQuery("SELECT * FROM Claims WHERE \"status\"='APPROVED' AND \"date\" > '2016-12-04'")}
    </div>

    <h2>Years Claims</h2>
    <br>
    <table>
        <tr>
            <th>ID</th>
            <th>Member ID</th>
            <th>Date</th>
            <th>Rationale</th>
            <th>Status</th>
            <th>Amount</th>
        </tr>
        <c:forEach items="${JDBCBean.resultsToArrayList()}" var="row" varStatus="rowStatus">
            <tr>
                <c:forEach items="${row}" var="column" varStatus="columnStatus">
                    <c:choose>
                        <c:when test="${columnStatus.last}">
                            <td>£${column}</td>                              
                        </c:when>
                        <c:otherwise>
                            <td>${column}</td>
                        </c:otherwise>
                    </c:choose>
                </c:forEach>
            </tr>
        </c:forEach>
    </table>
    <br><br>
    <h2>Total Claims: £${JDBCBean.sqlQueryToArrayList("SELECT SUM(\"amount\") FROM Claims WHERE \"status\"='APPROVED' AND \"date\" > '2016-12-04'")[0][0]}</h2>
    <h2>Number of active members: ${JDBCBean.sqlQueryToArrayList("SELECT COUNT(*) FROM members WHERE \"status\"='APPROVED'")[0][0]}</h2>
    
    <c:if test="${membersCharge ne null}">
        <h2>Members charged: £${membersCharge}</h2>      
    </c:if>
    <form action="${pageContext.request.contextPath}/AdminController" method="post">   
        <input type="hidden" name="viewId" value="/ManageTurnover">
        <button class="btn" type="submit" name="manageTurnoverAction" value="chargeMembers">Calculate Charges</button>
    </form>  
</div>